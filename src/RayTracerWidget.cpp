#include "RayTracerWidget.h"

void RayTracerWidget::updateSize(int dimension)
{
    if (dimension != _image.height())
    {
        _image = QImage(dimension, dimension, QImage::Format_RGB888);
        setFixedSize(dimension, dimension);
    }
}

void RayTracerWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0, 0, _image);
}
