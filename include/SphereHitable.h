#pragma once

#include <cmath>
#include <iostream>

#include <QDebug>

#include "Hitable.h"

class SphereHitable : public Hitable
{
private:
    float _radius;
    QVector3D _center;

public:
    const QVector3D & center() { return _center; }

    float radius() { return _radius; }

    SphereHitable(const QVector3D & pos, float radius) : _radius(radius), _center{pos} {}

    bool hit(const Ray & ray, float t_min, float t_max, HitRecord & rec) const override;

    bool hit(std::shared_ptr<SphereHitable> other)
    {
        return (_center - other->_center).length() < (_radius + other->_radius);
    }
};
