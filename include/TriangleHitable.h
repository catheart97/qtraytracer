#pragma once

#include <array>
#include <cmath>
#include <iostream>

#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

#include "Hitable.h"

class TriangleHitable : public Hitable
{
private:
    QVector3D _a, _b, _c;
    QVector3D _mass_center, _normal;
    std::array<QVector3D, 3> _edges;

public:
    template <typename index_t> const QVector3D & operator[](index_t index)
    {
        switch (index)
        {
            case 0: return a();
            case 1: return b();
            default: return c();
        }
    }

    const QVector3D & a() const { return _a; }

    const QVector3D & b() const { return _b; }

    const QVector3D & c() const { return _c; }

    const QVector3D & massCenter() const { return _mass_center; }

    TriangleHitable(const QVector3D & a, const QVector3D & b, const QVector3D & c)
        : _a{a}, _b{b}, _c{c}, _mass_center{(a + b + c) / 3.0f},
          _normal{QVector3D::crossProduct(_c - _a, _b - _a).normalized()}, _edges{b - a, c - b, a - c}
    {}

    QVector2D projectTo(const QVector3D & v) const;

    bool hit(const Ray & ray, float t_min, float t_max, HitRecord & rec) const override;
};
