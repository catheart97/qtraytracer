#include "SphereHitable.h"

bool SphereHitable::hit(const Ray & ray, float t_min, float t_max, HitRecord & rec) const
{
    using std::sqrt;

    QVector3D oc{ray.origin() - _center};
    float d_d = QVector3D::dotProduct(ray.direction(), ray.direction());
    float oc_d = QVector3D::dotProduct(oc, ray.direction());
    float oc_oc = QVector3D::dotProduct(oc, oc);
    float discriminant{oc_d * oc_d - d_d * (oc_oc - _radius * _radius)};

    if (discriminant > 0)
    {
        discriminant = sqrt(discriminant);

        float t = (-oc_d - discriminant) / d_d;
        if (t < t_max && t > t_min)
        {
            rec._t = t;
            rec._p = ray.point_at_parameter(t);
            rec._normal = (rec._p - _center) / _radius;
            rec._hit = this;
            return true;
        }

        t = (-oc_d + discriminant) / d_d;
        if (t < t_max && t > t_min)
        {
            rec._t = t;
            rec._p = ray.point_at_parameter(t);
            rec._normal = (rec._p - _center) / _radius;
            rec._hit = this;
            return true;
        }
    }
    return false;
}
