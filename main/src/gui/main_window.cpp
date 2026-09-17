#include "main_window.h"
#include "waveform.h"

#include <QApplication>
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QMimeData>
#include <QNetworkReply>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QTimer>
#include <QVBoxLayout>
#include <QDir>
#include <QUrlQuery>

namespace {
QLabel *makeLabel(const QString &text, const QString &className = QString()) {
    auto *label = new QLabel(text);
    if (!className.isEmpty()) label->setProperty("class", className);
    return label;
}

QPushButton *makeButton(const QString &text, const QString &className = QString()) {
    auto *button = new QPushButton(text);
    if (!className.isEmpty()) button->setProperty("class", className);
    return button;
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), recorder(this) {
    setWindowTitle("SwiftMusicSorting");
    setMinimumSize(1180, 760);
    resize(1440, 900);

    captureSession.setAudioInput(&audioInput);
    captureSession.setRecorder(&recorder);

    pythonProcess = new QProcess(this);
    connect(pythonProcess, &QProcess::readyReadStandardOutput, this, [this]() {
        pythonOutput += QString::fromUtf8(pythonProcess->readAllStandardOutput());
    });
    connect(pythonProcess, &QProcess::readyReadStandardError, this, [this]() {
        pythonOutput += QString::fromUtf8(pythonProcess->readAllStandardError());
    });
    connect(pythonProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
            this, &MainWindow::processFinished);
    connect(pythonProcess, &QProcess::errorOccurred, this, &MainWindow::processError);
    connect(&recorder, &QMediaRecorder::errorOccurred, this, [this](QMediaRecorder::Error, const QString &error) {
        setStatus("Recording error: " + error);
        waveform->setActive(false);
    });

    buildUi();
    applyStyle();
    resetSongCard();
}

void MainWindow::buildUi() {
    auto *root = new QWidget;
    auto *rootLayout = new QHBoxLayout(root);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // Sidebar
    auto *sidebar = new QWidget;
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(235);
    auto *side = new QVBoxLayout(sidebar);
    side->setContentsMargins(24, 28, 20, 24);
    side->setSpacing(8);

    auto *brand = makeLabel("SWIFT\nMUSIC", "brand");
    side->addWidget(brand);
    side->addSpacing(26);

    const QStringList nav = {"⌂   Home", "◉   Recognize", "♫   Your Library", "↺   History", "⚙   Settings"};
    for (int i = 0; i < nav.size(); ++i) {
        auto *b = makeButton(nav[i], i == 1 ? "nav active" : "nav");
        b->setCursor(Qt::PointingHandCursor);
        side->addWidget(b);
    }

    side->addStretch();
    auto *quote = makeLabel("Music is what\nfeelings sound like.", "quote");
    quote->setWordWrap(true);
    side->addWidget(quote);
    side->addSpacing(8);
    side->addWidget(makeLabel("Made with ♥ by Aman Naveen", "madeBy"));
    rootLayout->addWidget(sidebar);

    // Main content
    auto *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    auto *content = new QWidget;
    auto *main = new QVBoxLayout(content);
    main->setContentsMargins(34, 28, 34, 36);
    main->setSpacing(20);

    auto *header = new QHBoxLayout;
    auto *headerText = new QVBoxLayout;
    headerText->setSpacing(3);
    headerText->addWidget(makeLabel("What’s playing?", "pageTitle"));
    headerText->addWidget(makeLabel("Drop a track. We’ll tell you what it is.", "pageSubtitle"));
    header->addLayout(headerText);
    header->addStretch();
    statusLabel = makeLabel("READY", "statusPill");
    header->addWidget(statusLabel, 0, Qt::AlignTop);
    main->addLayout(header);

    // Listening card
    auto *listenCard = new QWidget;
    listenCard->setObjectName("listenCard");
    auto *listen = new QVBoxLayout(listenCard);
    listen->setContentsMargins(28, 24, 28, 24);
    listen->setSpacing(14);

    auto *listenTop = new QHBoxLayout;
    auto *listenTitle = new QVBoxLayout;
    listenTitle->addWidget(makeLabel("LISTENING STATION", "eyebrow"));
    fileLabel = makeLabel("No track selected", "fileName");
    listenTitle->addWidget(fileLabel);
    listenTop->addLayout(listenTitle);
    listenTop->addStretch();
    timerLabel = makeLabel("00:00", "timer");
    listenTop->addWidget(timerLabel);
    listen->addLayout(listenTop);

    waveform = new WaveformWidget;
    listen->addWidget(waveform);

    auto *actions = new QHBoxLayout;
    actions->setSpacing(10);
    uploadButton = makeButton("＋  Upload File", "primary");
    recognizeButton = makeButton("✦  Recognize", "secondary");
    micButton = makeButton("●  Use Microphone", "ghost");
    stopButton = makeButton("■  Stop", "danger");
    for (auto *b : {uploadButton, recognizeButton, micButton, stopButton}) {
        b->setCursor(Qt::PointingHandCursor);
        actions->addWidget(b);
    }
    actions->addStretch();
    listen->addLayout(actions);
    main->addWidget(listenCard);

    // Feature cards
    auto *featureHeader = new QHBoxLayout;
    featureHeader->addWidget(makeLabel("Audio Features", "sectionTitle"));
    featureHeader->addStretch();
    featureHeader->addWidget(makeLabel("LIBROSA ANALYSIS", "sectionTag"));
    main->addLayout(featureHeader);

    auto *features = new QHBoxLayout;
    const QStringList names = {"TEMPO", "INTENSITY", "SPECTRAL CENTROID", "ZERO-CROSSING RATE"};
    QLabel **values[] = {&tempoValue, &intensityValue, &centroidValue, &zcrValue};
    for (int i = 0; i < 4; ++i) {
        auto *card = new QWidget;
        card->setObjectName("featureCard");
        auto *v = new QVBoxLayout(card);
        v->setContentsMargins(18, 16, 18, 16);
        v->setSpacing(6);
        v->addWidget(makeLabel(names[i], "featureName"));
        *values[i] = makeLabel("—", "featureValue");
        v->addWidget(*values[i]);
        features->addWidget(card);
    }
    main->addLayout(features);

    // Genre row
    auto *genreCard = new QWidget;
    genreCard->setObjectName("genreCard");
    auto *genreLayout = new QHBoxLayout(genreCard);
    genreLayout->setContentsMargins(20, 16, 20, 16);
    genreLayout->addWidget(makeLabel("GENRE ANALYSIS", "featureName"));
    genreValue = makeLabel("Waiting for audio", "genreValue");
    genreLayout->addWidget(genreValue);
    genreLayout->addStretch();
    genreLayout->addWidget(makeLabel("Tempo + spectral features", "muted"));
    main->addWidget(genreCard);

    scroll->setWidget(content);
    rootLayout->addWidget(scroll, 1);

    // Right result panel
    auto *right = new QWidget;
    right->setObjectName("rightPanel");
    right->setFixedWidth(330);
    auto *r = new QVBoxLayout(right);
    r->setContentsMargins(24, 28, 24, 28);
    r->setSpacing(14);

    r->addWidget(makeLabel("MATCH RESULT", "eyebrow"));
    coverLabel = makeLabel;
    coverLabel->setObjectName("cover");
    coverLabel->setFixedSize(282, 282);
    coverLabel->setAlignment(Qt::AlignCenter);
    r->addWidget(coverLabel, 0, Qt::AlignCenter);

    matchLabel = makeLabel("—", "match");
    matchLabel->setAlignment(Qt::AlignCenter);
    r->addWidget(matchLabel);
    songTitle = makeLabel("Nothing recognized yet", "songTitle");
    songTitle->setWordWrap(true);
    songTitle->setAlignment(Qt::AlignCenter);
    r->addWidget(songTitle);
    artistLabel = makeLabel("Upload or record a track", "artist");
    artistLabel->setAlignment(Qt::AlignCenter);
    r->addWidget(artistLabel);
    albumLabel = makeLabel("", "album");
    albumLabel->setAlignment(Qt::AlignCenter);
    r->addWidget(albumLabel);

    spotifyButton = makeButton("Open in Spotify  ↗", "spotify");
    spotifyButton->setCursor(Qt::PointingHandCursor);
    r->addWidget(spotifyButton);
    r->addStretch();

    auto *hint = makeLabel("Tip  ·  Use a clear 10–30 sec clip for faster recognition.", "hint");
    hint->setWordWrap(true);
    r->addWidget(hint);
    rootLayout->addWidget(right);

    setCentralWidget(root);

    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::chooseFile);
    connect(recognizeButton, &QPushButton::clicked, this, &MainWindow::recognizeCurrentFile);
    connect(micButton, &QPushButton::clicked, this, &MainWindow::startMicrophone);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopMicrophone);
    connect(spotifyButton, &QPushButton::clicked, this, [this]() {
        if (!spotifyUrl.isEmpty()) QDesktopServices::openUrl(QUrl(spotifyUrl));
    });
}

void MainWindow::applyStyle() {
    QString path = QDir(QCoreApplication::applicationDirPath()).filePath("style.qss");
    QStringList candidates = {
        path,
        QDir(QCoreApplication::applicationDirPath()).filePath("../src/gui/style.qss"),
        QDir::current().filePath("src/gui/style.qss"),
        QDir::current().filePath("main/src/gui/style.qss")
    };
    QFile file;
    for (const auto &candidate : candidates) {
        file.setFileName(candidate);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) break;
    }
    if (file.isOpen()) qApp->setStyleSheet(QString::fromUtf8(file.readAll()));
}

QString MainWindow::findPythonScript(const QString &name) const {
    const QStringList candidates = {
        QDir::current().filePath("python/" + name),
        QDir::current().filePath("main/python/" + name),
        QDir(QCoreApplication::applicationDirPath()).filePath("../python/" + name),
        QDir(QCoreApplication::applicationDirPath()).filePath("../../python/" + name)
    };
    for (const auto &path : candidates) if (QFileInfo::exists(path)) return QFileInfo(path).absoluteFilePath();
    return {};
}

void MainWindow::chooseFile() {
    const QString path = QFileDialog::getOpenFileName(this, "Choose an audio file", QString(),
        "Audio files (*.wav *.mp3 *.flac *.m4a *.aac *.ogg);;All files (*.*)");
    if (path.isEmpty()) return;
    currentFile = path;
    fileLabel->setText(QFileInfo(path).fileName());
    resetSongCard();
    setStatus("FILE READY");
}

void MainWindow::recognizeCurrentFile() {
    if (currentFile.isEmpty()) {
        chooseFile();
        if (currentFile.isEmpty()) return;
    }
    runPython("recognize_song.py", currentFile);
}

void MainWindow::runPython(const QString &script, const QString &filePath) {
    const QString scriptPath = findPythonScript(script);
    if (scriptPath.isEmpty()) {
        setStatus("PYTHON SCRIPT NOT FOUND");
        return;
    }
    if (pythonProcess->state() != QProcess::NotRunning) return;

    pythonOutput.clear();
    waveform->setActive(true);
    setStatus("ANALYZING…", true);
    recognizeButton->setEnabled(false);
    uploadButton->setEnabled(false);
    micButton->setEnabled(false);
    pythonProcess->setWorkingDirectory(QFileInfo(scriptPath).absolutePath());
    pythonProcess->start("python", {scriptPath, filePath});
}

void MainWindow::processFinished(int exitCode, QProcess::ExitStatus status) {
    Q_UNUSED(status);
    waveform->setActive(false);
    recognizeButton->setEnabled(true);
    uploadButton->setEnabled(true);
    micButton->setEnabled(true);
    setStatus(exitCode == 0 ? "DONE" : "ERROR");

    if (pythonOutput.contains("Title:") || pythonOutput.contains("Recognition:")) {
        parseRecognitionOutput(pythonOutput);
    }
    if (pythonOutput.contains("Tempo:") || pythonOutput.contains("Genre estimate:")) {
        parseFeatureOutput(pythonOutput);
    }
}

void MainWindow::processError(QProcess::ProcessError error) {
    if (error == QProcess::FailedToStart) setStatus("PYTHON NOT FOUND");
}

void MainWindow::parseRecognitionOutput(const QString &output) {
    auto value = [&output](const QString &key) {
        QRegularExpression re("^" + QRegularExpression::escape(key) + "\\s*(.*)$",
                              QRegularExpression::MultilineOption);
        const auto match = re.match(output);
        return match.hasMatch() ? match.captured(1).trimmed() : QString();
    };

    const QString title = value("Title:");
    const QString artist = value("Artist:");
    const QString album = value("Album:");
    spotifyUrl = value("Spotify:");
    const QString cover = value("Album cover:");

    if (!title.isEmpty()) songTitle->setText(title);
    if (!artist.isEmpty()) artistLabel->setText(artist);
    if (!album.isEmpty()) albumLabel->setText(album);

    if (!title.isEmpty() && title != "Unknown") {
        matchLabel->setText("MATCH FOUND");
    } else {
        matchLabel->setText("NO MATCH");
    }
    spotifyButton->setEnabled(!spotifyUrl.isEmpty() && spotifyUrl != "Not available");
    if (!cover.isEmpty() && cover != "Not available") showCover(QUrl(cover));
}

void MainWindow::parseFeatureOutput(const QString &output) {
    auto value = [&output](const QString &key) {
        QRegularExpression re("^" + QRegularExpression::escape(key) + "\\s*(.*)$",
                              QRegularExpression::MultilineOption);
        const auto match = re.match(output);
        return match.hasMatch() ? match.captured(1).trimmed() : QString();
    };

    const QString tempo = value("Tempo:");
    const QString intensity = value("Intensity:");
    const QString genre = value("Genre estimate:");
    const QString centroid = value("Spectral centroid:");
    const QString zcr = value("Zero-crossing rate:");

    if (!tempo.isEmpty()) tempoValue->setText(tempo);
    if (!intensity.isEmpty()) intensityValue->setText(intensity);
    if (!genre.isEmpty()) genreValue->setText(genre);
    if (!centroid.isEmpty()) centroidValue->setText(centroid);
    if (!zcr.isEmpty()) zcrValue->setText(zcr);
}

void MainWindow::resetSongCard() {
    spotifyUrl.clear();
    songTitle->setText("Nothing recognized yet");
    artistLabel->setText("Upload or record a track");
    albumLabel->clear();
    matchLabel->setText("—");
    spotifyButton->setEnabled(false);
    coverLabel->setText("♪");
    coverLabel->setPixmap(QPixmap());
    tempoValue->setText("—");
    intensityValue->setText("—");
    centroidValue->setText("—");
    zcrValue->setText("—");
    genreValue->setText("Waiting for audio");
}

void MainWindow::showCover(const QUrl &url) {
    auto *reply = network.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, &MainWindow::coverFinished);
}

void MainWindow::coverFinished() {
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    const QByteArray data = reply->readAll();
    QPixmap pix;
    if (pix.loadFromData(data)) {
        coverLabel->setText("");
        coverLabel->setPixmap(pix.scaled(282, 282, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }
    reply->deleteLater();
}

void MainWindow::startMicrophone() {
    if (recorder.recorderState() == QMediaRecorder::RecordingState) return;

    recordingFile = QDir(QDir::tempPath()).filePath("swiftmusic_recording.m4a");
    QFile::remove(recordingFile);
    recorder.setOutputLocation(QUrl::fromLocalFile(recordingFile));
    recorder.record();
    waveform->setActive(true);
    timerLabel->setText("● LIVE");
    setStatus("LISTENING", true);
}

void MainWindow::stopMicrophone() {
    if (recorder.recorderState() != QMediaRecorder::RecordingState) return;
    recorder.stop();
    waveform->setActive(false);
    timerLabel->setText("00:00");
    currentFile = recordingFile;
    fileLabel->setText("Microphone recording");
    QTimer::singleShot(500, this, [this]() {
        if (QFileInfo::exists(currentFile)) recognizeCurrentFile();
    });
}

QString MainWindow::formatNumber(double value, int decimals) const {
    return QString::number(value, 'f', decimals);
}
