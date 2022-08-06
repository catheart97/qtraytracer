#pragma once

#include <random>

#include "Constants.h"
#include "Ray.h"

class Camera
{
private:
    int _pixel_dimension;

public:
    virtual ~Camera() = default;

    virtual Ray getRay(float x, float y) = 0;

    virtual int getPixelDimension() { return _pixel_dimension; }

    virtual void setPixelDimension(int pixel_dimension) { _pixel_dimension = pixel_dimension; }
};
