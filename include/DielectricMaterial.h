#pragma once

#include <iostream>

#include <QColor>
#include <QVector3D>

#include "Material.h"
#include "RayTracer.h"
#include "Utils.h"

class DielectricMaterial : public Material
{
private:
    QVector3D _a{0.25f, 0.25f, 0.25f};
    float _n{1.5f}; // refraction index

public:
    DielectricMaterial(float n, QVector3D a = {.2f, .2f, .2f}) : _a{a}, _n{n} {}

    QVector3D getColor(const Ray & ray_in, const HitRecord & rec, const RayTracer * tracer,
                       int depth) override;
};
