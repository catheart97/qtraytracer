#pragma once

#include <vector>

#include <QVector3D>

#include <Eigen/Dense>

#include "BVH.h"
#include "Hitable.h"
#include "SphereHitable.h"
#include "TriangleHitable.h"

class TriangleMeshHitable : public Hitable
{
private:
    std::vector<std::shared_ptr<TriangleHitable>> _triangles;
    std::unique_ptr<BVH> _root;

public:
    TriangleMeshHitable(const std::vector<std::shared_ptr<TriangleHitable>> & triangles, bool bvh = true)
        : _triangles{triangles}
    {
        if (bvh)
        {
            _root = std::make_unique<BVH>(triangles);
        }
    }

    bool hit(const Ray & ray, float t_min, float t_max, HitRecord & rec) const override
    {
        if (_root)
            return _root->hit(ray, t_min, t_max, rec);
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
};
