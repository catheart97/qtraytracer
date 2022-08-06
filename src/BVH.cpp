#include "BVH.h"

bool BVH::hit(const Ray & ray, float t_min, float t_max, HitRecord & rec) const
{
    if (_bounding_sphere->hit(ray, t_min, t_max, rec))
    {
        if (_left && _right)
        {
            HitRecord l, r;
            bool ls{false}, rs{false};
            if (_left->hit(ray, t_min, t_max, l)) ls = true;
            if (_right->hit(ray, t_min, t_max, r)) rs = true;

            if (ls && rs)
                rec = (l._t < r._t) ? l : r;
            else if (ls)
                rec = l;
            else if (rs)
                rec = r;
            else
                return false;

            return true;
        }
        else
        {
            bool hit_{false};
            for (auto t : _triangles)
            {
                HitRecord rt_;
                if (t->hit(ray, t_min, t_max, rt_))
                {
                    if (!hit_)
                    {
                        hit_ = true;
                        rec = rt_;
                    }
                    else if (rec._t > rt_._t)
                        rec = rt_;
                }
            }
            return hit_;
        }
    }
    else
    {
        return false;
    }
}

BVH::BVH(const std::vector<std::shared_ptr<TriangleHitable>> & triangles)
{

    QVector3D center;
    for (const std::shared_ptr<TriangleHitable> & t : triangles) center += t->a() + t->b() + t->c();
    center /= (triangles.size() * 3);

    float radius = 0.0f;
    for (const std::shared_ptr<TriangleHitable> & t : triangles)
    {
        radius = std::max((t->a() - center).lengthSquared(), radius);
        radius = std::max((t->b() - center).lengthSquared(), radius);
        radius = std::max((t->c() - center).lengthSquared(), radius);
    }
    radius = std::sqrt(radius);

    _bounding_sphere = std::make_unique<SphereHitable>(center, radius);
    _left = nullptr;
    _right = nullptr;

    if (triangles.size() < 30)
        _triangles = triangles;
    else
        generateChildren(triangles);
}

void BVH::generateChildren(std::vector<std::shared_ptr<TriangleHitable>> triangles)
{
    using namespace Eigen;

    QVector3D mass_center{_bounding_sphere->center()};
    QMatrix3x3 inertia;
    inertia.fill(0.0);
    // Compute inertia matrix
    for (auto t : triangles)
    {
        for (auto i = 0; i < 3; ++i)
        {
            const QVector3D & r = t->operator[](i) - mass_center;
            inertia(0, 0) += r[1] * r[1] + r[2] * r[2];
            inertia(1, 1) += r[0] * r[0] + r[2] * r[2];
            inertia(2, 2) += r[0] * r[0] + r[1] * r[1];
            inertia(0, 1) -= r[0] * r[1];
            inertia(0, 2) -= r[0] * r[2];
            inertia(1, 2) -= r[1] * r[2];
        }
    }
    inertia(1, 0) = inertia(0, 1);
    inertia(2, 0) = inertia(0, 2);
    inertia(2, 1) = inertia(1, 2);

    std::vector<std::shared_ptr<TriangleHitable>> left;
    std::vector<std::shared_ptr<TriangleHitable>> right;

    Matrix3d M_e = toEigenMatrix(inertia);
    Eigen::EigenSolver<Matrix3d> es(M_e);
    auto ev = es.eigenvectors();

    int m = -1;
    es.eigenvalues().real().minCoeff(&m);
    QVector3D v = toQVector3D(ev.col(m).real());

    std::sort(triangles.begin(), triangles.end(),
              [&v](const std::shared_ptr<TriangleHitable> & t1, const std::shared_ptr<TriangleHitable> & t2) {
                  return QVector3D::dotProduct(t1->massCenter(), v) <
                         QVector3D::dotProduct(t2->massCenter(), v);
              });

    for (size_t i = 0; i < triangles.size() / 2; ++i) left.push_back(triangles[i]);
    for (size_t i = triangles.size() / 2; i < triangles.size(); ++i) right.push_back(triangles[i]);

    _left = std::make_unique<BVH>(left);
    _right = std::make_unique<BVH>(right);
}
