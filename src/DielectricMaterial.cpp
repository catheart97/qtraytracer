#include "DielectricMaterial.h"

QVector3D DielectricMaterial::getColor(const Ray & ray_in, const HitRecord & rec, const RayTracer * tracer,
                                       int depth)
{
    if (depth > BOUNCE_DEPTH) return tracer->background(ray_in);

    float cosine;
    QVector3D r{reflect(ray_in.direction(), rec._normal)}, d{ray_in.direction().normalized()},
        n{rec._normal.normalized()}, k{1, 1, 1}, t;

    if (QVector3D::dotProduct(d, n) < 0)
    {
        if (!refract(-d, n, 1.0f / _n, t)) return {1, 0, 0};
        cosine = -QVector3D::dotProduct(d, n);
    }
    else
    {
        k[0] = std::expf(-_a[0] * rec._t);
        k[1] = std::expf(-_a[1] * rec._t);
        k[2] = std::expf(-_a[2] * rec._t);

        if (refract(d, -n, 1 / _n, t))
            cosine = QVector3D::dotProduct(d, n);
        else
            return k * tracer->getColor(Ray(rec._p, r), depth + 1);
    }

    float R0 = (_n - 1) / (_n + 1);
    R0 *= R0;
    float R = R0 + (1 - R0) * (std::pow(1 - cosine, 5));

    if (randomF() < R)
    {
        return k * tracer->getColor(Ray(rec._p, r), depth + 1);
    }
    else
    {
        return k * tracer->getColor(Ray(rec._p, t), depth + 1);
    }
}
