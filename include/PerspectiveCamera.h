#pragma once

#include <cmath>
#include <vector>

#include <QDebug>
#include <QMatrix4x4>
#include <QVector3D>

#include "Camera.h"
#include "Constants.h"
#include "Utils.h"

class PerspectiveCamera : public Camera
{
private:
    QMatrix4x4 _view;
    float _half_dimension; // l,r,b,t
    float _dimension;      // l+r and b+t

    QVector3D _pos;       // eyepoint
    QVector3D _x, _y, _z; // coordinate system
    float _d;             // distance to image plane

    // Depth of field stuff
    float _f{2.f};
    float _aperture{0.01f};

public:
    PerspectiveCamera(QVector3D pos, QVector3D view_direction, QVector3D up, float d, float dimension);

    void setFocusPlane(float f) { _f = f; }

    void setAperture(float aperture) { _aperture = aperture; }

    float getFocus() { return _f; }

    float getAperture() { return _aperture; }

    /**
     * Get Ray for Pixel x, y
     * @param[x] x-pixel coordinate (include .5 for single or random sampling jittering)
     * @param[y] y-pixel coordinate (include .5 for single or random sampling jittering)
     */
    Ray getRay(float x, float y) override;
};
