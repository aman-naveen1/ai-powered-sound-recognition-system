#include "waveform.h"

#include <QPainter>
#include <QtMath>

WaveformWidget::WaveformWidget(QWidget *parent) : QWidget(parent) {
    setMinimumHeight(96);
    bars.resize(48);
    bars.fill(0.12f);

    connect(&timer, &QTimer::timeout, this, [this]() {
        phase += 0.22;
        for (int i = 0; i < bars.size(); ++i) {
            const double wave = 0.18 + 0.70 * qAbs(qSin(phase + i * 0.48));
            const double pulse = 0.75 + 0.25 * qSin(phase * 1.7 + i);
            bars[i] = active ? static_cast<float>(qBound(0.08, wave * pulse, 1.0)) : 0.08f;
        }
        update();
    });
    timer.start(35);
}

void WaveformWidget::setActive(bool value) {
    active = value;
    update();
}

void WaveformWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::transparent);

    const int count = bars.size();
    const double gap = 4.0;
    const double width = (rect().width() - gap * (count - 1)) / count;
    const double center = rect().height() / 2.0;

    for (int i = 0; i < count; ++i) {
        const double height = qMax(5.0, bars[i] * rect().height() * 0.82);
        QRectF bar(i * (width + gap), center - height / 2.0, width, height);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 55, 95, active ? 235 : 80));
        painter.drawRoundedRect(bar, width / 2.0, width / 2.0);
    }
}
