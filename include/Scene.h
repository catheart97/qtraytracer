#pragma once

#include <memory>
#include <vector>

#include "Hitable.h"
#include "Ray.h"

class Scene : public Hitable
{
private:
    std::vector<std::shared_ptr<Hitable>> _hitables;

public:
    void addHitable(std::shared_ptr<Hitable> hitable) { _hitables.push_back(hitable); }

    bool hit(const Ray & ray, float t_min, float t_max, HitRecord & rec) const override
    {
        HitRecord tmp_rec;
        bool hit_any{false};
        float t_closest{t_max};

        for (auto hitable : _hitables)
        {
            if (hitable->hit(ray, t_min, t_closest, tmp_rec))
            {
                t_closest = tmp_rec._t;
                rec = tmp_rec;
                hit_any = true;
            }
        }

        return hit_any;
    }
};
