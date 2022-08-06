#include "DiffuseMaterial.h"

DiffuseMaterial::DiffuseMaterial(const QColor & color)
{
    _albedo = {float(color.red()), float(color.green()), float(color.blue())};
    _albedo /= 255.f;
}

QVector3D DiffuseMaterial::getColor(const Ray & ray_in, const HitRecord & rec, const RayTracer * tracer,
                                    int depth)
{
    if (depth > BOUNCE_DEPTH) return tracer->background(ray_in);
    QVector3D target{rec._p + rec._normal + randomPointInUnitSphere()};
    return _albedo * tracer->getColor(Ray(rec._p, QVector3D{target - rec._p}.normalized()), depth + 1);
}
