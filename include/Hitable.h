#pragma once

#include <memory>

#include "Ray.h"

class Hitable;
class Material;

struct HitRecord
{
    float _t;
    QVector3D _p;
    QVector3D _normal;
    const Hitable * _hit;
};

class Hitable
{
private:
    std::shared_ptr<Material> _material;

public:
    virtual ~Hitable() = default;

    virtual std::shared_ptr<Material> getMaterial() const { return _material; }

    virtual void setMaterial(std::shared_ptr<Material> material) { _material = material; }

    virtual bool hit(const Ray & ray, float t_min, float t_max, HitRecord & rec) const = 0;
};
