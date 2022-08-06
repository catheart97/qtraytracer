#pragma once

#include <QColor>
#include <QVector3D>

#include "Material.h"
#include "RayTracer.h"
#include "Utils.h"

class MetalMaterial : public Material
{
private:
    QVector3D _albedo;
    float _fuzz{0.f};

public:
    MetalMaterial(const QVector3D & albedo, float fuzz) : _albedo{albedo}, _fuzz{fuzz} {}
    MetalMaterial(const QVector3D & albedo) : _albedo{albedo} {}

    MetalMaterial(const QColor & color)
    {
        _albedo = {float(color.red()), float(color.green()), float(color.blue())};
        _albedo /= 255.f;
    }

    void setFuzz(float fuzz) { _fuzz = fuzz; }

    QVector3D getColor(const Ray & ray_in, const HitRecord & rec, const RayTracer * tracer,
                       int depth) override;
};
