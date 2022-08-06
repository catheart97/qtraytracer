#pragma once

#include <QVector3D>

class Ray
{
private:
    QVector3D _a, _d;

public:
    Ray() = default;

    Ray(const QVector3D & a, const QVector3D & d) : _a{std::move(a)}, _d{std::move(d)} {}

    const QVector3D & origin() const { return _a; }

    const QVector3D & direction() const { return _d; }

    void setOrigin(const QVector3D & origin) { _a = origin; }

    void setDirection(const QVector3D & direction) { _d = direction; }

    QVector3D point_at_parameter(float t) const { return _a + t * _d; }
};
