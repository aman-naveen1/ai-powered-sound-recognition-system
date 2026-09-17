#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QWidget>
#include <QVector>
#include <QTimer>

class WaveformWidget : public QWidget {
    Q_OBJECT

public:
    explicit WaveformWidget(QWidget *parent = nullptr);
    void setActive(bool active);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTimer timer;
    QVector<float> bars;
    bool active = false;
    double phase = 0.0;
};

#endif
