#include "MetalMaterial.h"

QVector3D MetalMaterial::getColor(const Ray & ray_in, const HitRecord & rec, const RayTracer * tracer,
                                  int depth)
{
    Ray ray_scattered(rec._p, reflect(ray_in.direction().normalized(), rec._normal) +
                                  _fuzz * randomPointInUnitSphere());
    if (depth < BOUNCE_DEPTH && QVector3D::dotProduct(rec._normal, ray_scattered.direction()) > 0)
        return _albedo * tracer->getColor(ray_scattered, depth + 1);
    else
        return _albedo * tracer->background(ray_scattered);
}
