#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QAudioInput>
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QProcess>
#include <QUrl>

class QLabel;
class QPushButton;
class QProgressBar;
class WaveformWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void chooseFile();
    void recognizeCurrentFile();
    void startMicrophone();
    void stopMicrophone();
    void processFinished(int exitCode, QProcess::ExitStatus status);
    void processError(QProcess::ProcessError error);
    void coverFinished();

private:
    void buildUi();
    void applyStyle();
    void runPython(const QString &script, const QString &filePath);
    QString findPythonScript(const QString &name) const;
    void setStatus(const QString &text, bool busy = false);
    void parseRecognitionOutput(const QString &output);
    void parseFeatureOutput(const QString &output);
    void resetSongCard();
    void showCover(const QUrl &url);
    QString formatNumber(double value, int decimals = 0) const;

    QLabel *statusLabel = nullptr;
    QLabel *fileLabel = nullptr;
    QLabel *songTitle = nullptr;
    QLabel *artistLabel = nullptr;
    QLabel *albumLabel = nullptr;
    QLabel *coverLabel = nullptr;
    QLabel *matchLabel = nullptr;
    QLabel *tempoValue = nullptr;
    QLabel *intensityValue = nullptr;
    QLabel *centroidValue = nullptr;
    QLabel *zcrValue = nullptr;
    QLabel *genreValue = nullptr;
    QLabel *timerLabel = nullptr;
    QPushButton *uploadButton = nullptr;
    QPushButton *recognizeButton = nullptr;
    QPushButton *micButton = nullptr;
    QPushButton *stopButton = nullptr;
    QPushButton *spotifyButton = nullptr;
    WaveformWidget *waveform = nullptr;

    QProcess *pythonProcess = nullptr;
    QNetworkAccessManager network;
    QMediaCaptureSession captureSession;
    QAudioInput audioInput;
    QMediaRecorder recorder;
    QString currentFile;
    QString spotifyUrl;
    QString pythonOutput;
    QString recordingFile;
};

#endif
