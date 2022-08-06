#pragma once

#include <QImage>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>

#include "RayTracerDrawable.h"

class RayTracerWidget : public QWidget, public RayTracerDrawable
{
    Q_OBJECT

private:
    QImage _image;

public:
    RayTracerWidget(QWidget * parent)
        : QWidget(parent), _image(DIMENSION, DIMENSION, QImage::QImage::Format_RGB888)
    {
        setFixedSize(DIMENSION, DIMENSION);
    }

    void updateSize(int dimension) override;

    void setPixel(int x, int y, const QColor & color) noexcept override { _image.setPixelColor(x, y, color); }

    QColor getPixel(int x, int y) const noexcept override { return _image.pixel(x, y); }

    void updateScreen() override { update(); }

    void paintEvent(QPaintEvent *) override;

    QImage getImage() const { return _image; }
};
