#include "TriangleHitable.h"

QVector2D TriangleHitable::projectTo(const QVector3D & v) const
{
    return {std::min({QVector3D::dotProduct(a(), v), QVector3D::dotProduct(b(), v),
                      QVector3D::dotProduct(c(), v)}),
            std::max({QVector3D::dotProduct(a(), v), QVector3D::dotProduct(b(), v),
                      QVector3D::dotProduct(c(), v)})};
}

bool TriangleHitable::hit(const Ray & ray, float t_min, float t_max, HitRecord & rec) const
{
    QMatrix4x4 A;
    A.setColumn(0, QVector4D(_a - _b, 0));
    A.setColumn(1, QVector4D(_a - _c, 0));
    A.setColumn(2, QVector4D(ray.direction(), 0));
    A.setColumn(3, QVector4D(0, 0, 0, 1));

    QVector3D q{_a - ray.origin()};

    float a22a33_a23a32{A(1, 1) * A(2, 2) - A(1, 2) * A(2, 1)};
    float a13a32_a12a33{A(0, 2) * A(2, 1) - A(0, 1) * A(2, 2)};
    float a12a23_a22a13{A(0, 1) * A(1, 2) - A(1, 1) * A(0, 2)};

    float a11qy_qxa21{A(0, 0) * q.y() - q.x() * A(1, 0)};
    float qxa31_a11qz{q.x() * A(2, 0) - A(0, 0) * q.z()};
    float a21qz_qya31{A(1, 0) * q.z() - q.y() * A(2, 0)};

    float M{A(0, 0) * a22a33_a23a32 + A(1, 0) * a13a32_a12a33 + A(2, 0) * a12a23_a22a13};
    if (std::abs(M) < 0.0001f) return false;

    float t{-(A(2, 1) * a11qy_qxa21 + A(1, 1) * qxa31_a11qz + A(0, 1) * a21qz_qya31) / M};
    if (t < t_min || t > t_max) return false;

    float gamma{(A(2, 2) * a11qy_qxa21 + A(1, 2) * qxa31_a11qz + A(0, 2) * a21qz_qya31) / M};
    if (gamma < 0 || gamma > 1) return false;

    float beta{(q.x() * a22a33_a23a32 + q.y() * a13a32_a12a33 + q.z() * a12a23_a22a13) / M};
    if (beta < 0 || beta > 1 - gamma) // beta and gamma need to sum up to one
        return false;

    rec._p = ray.point_at_parameter(t);
    rec._t = t;
    rec._hit = this;
    rec._normal = _normal;
    if (QVector3D::dotProduct(rec._normal, ray.direction()) > 0) rec._normal *= -1.f;

    return true;
}
