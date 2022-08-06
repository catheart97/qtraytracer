#pragma once

#include <QObject>

#include "Material.h"

class EmissionMaterial : public Material
{
private:
    QVector3D _color;

public:
    EmissionMaterial(QVector3D color = {1, 1, 1}) : _color{color} {}

    QVector3D getColor(const Ray &, const HitRecord &, const RayTracer *, int) { return _color; }
};
