#pragma once

#include <QColor>

#include "Constants.h"

class RayTracerDrawable
{
public:
    virtual ~RayTracerDrawable() = default;

    virtual void updateSize(int dimension) = 0;

    virtual void setPixel(int x, int y, const QColor & color) noexcept = 0;

    virtual QColor getPixel(int x, int y) const noexcept = 0;

    virtual void updateScreen() = 0;
};
