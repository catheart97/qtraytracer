#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(QVector3D pos, QVector3D view_direction, QVector3D up, float d,
                                     float dimension)
    : Camera(), _half_dimension{std::abs(dimension) / 2.f},
      _dimension{std::abs(dimension)}, _pos{pos}, _z{-view_direction}, _d{d}
{
    _x = QVector3D::crossProduct(up, _z);
    _y = QVector3D::crossProduct(_x, view_direction);

    _view = {_x[0], _y[0], _z[0], 0, _x[1], _y[1], _z[1], 0, _x[2], _y[2], _z[2], 0, 0, 0, 0, 1};
}

Ray PerspectiveCamera::getRay(float x, float y)
{
    if (_f == 0.0f) // field of depth disabled
    {
        QVector3D s{-_half_dimension + _dimension / float(getPixelDimension()) * x,
                    -_half_dimension + _dimension / float(getPixelDimension()) * y, -_d};
        return Ray(_pos, _view.mapVector(s));
    }
    else
    {
        // transform screen dimension to focus plane
        float focus_dimension{_dimension / _d * _f}, focus_half_dimension{focus_dimension / 2};

        auto offset{_view.mapVector(_aperture * randomPointInSquare())};

        QVector3D s{-focus_half_dimension + focus_dimension / float(getPixelDimension()) * x,
                    -focus_half_dimension + focus_dimension / float(getPixelDimension()) * y, -_f};

        return Ray(offset + _pos, _view.mapVector(s - offset));
    }
}
