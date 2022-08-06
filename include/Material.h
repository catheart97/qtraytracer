#pragma once

#include <vector>

#include <QVector3D>

#include "Hitable.h"
#include "Ray.h"

class RayTracer;

class Material
{
public:
    virtual ~Material() = default;

    virtual QVector3D getColor(const Ray & ray_in, const HitRecord & rec, const RayTracer * tracer,
                               int depth) = 0;
};
