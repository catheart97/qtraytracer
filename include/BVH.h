#pragma once

#include <Eigen/Dense>

#include "Hitable.h"
#include "SphereHitable.h"
#include "TriangleHitable.h"

inline Eigen::Matrix3d toEigenMatrix(const QMatrix3x3 & m)
{
    Eigen::Matrix3d M;
    M << double(m(0, 0)), double(m(0, 1)), double(m(0, 2)), double(m(1, 0)), double(m(1, 1)), double(m(1, 2)),
        double(m(2, 0)), double(m(2, 1)), double(m(2, 2));
    return M;
}

inline QVector3D toQVector3D(const Eigen::Vector3d & v)
{
    return QVector3D(float(v[0]), float(v[1]), float(v[2]));
}

class BVH : public Hitable
{
private:
    std::unique_ptr<SphereHitable> _bounding_sphere;
    std::vector<std::shared_ptr<TriangleHitable>> _triangles;

    std::unique_ptr<BVH> _left;
    std::unique_ptr<BVH> _right;

public:
    bool hit(const Ray & ray, float t_min, float t_max, HitRecord & rec) const override;

    BVH(const std::vector<std::shared_ptr<TriangleHitable>> & triangles);

private:
    void generateChildren(std::vector<std::shared_ptr<TriangleHitable>> triangles);
};
