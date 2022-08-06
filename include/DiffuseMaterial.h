#pragma once

#include <QColor>
#include <QVector3D>

#include "Material.h"
#include "RayTracer.h"
#include "Utils.h"

class DiffuseMaterial : public Material
{
private:
    QVector3D _albedo;

public:
    DiffuseMaterial(const QVector3D & albedo) : _albedo{albedo} {}

    DiffuseMaterial(const QColor & color);

    QVector3D getColor(const Ray & ray_in, const HitRecord & rec, const RayTracer * tracer,
                       int depth) override;
};
