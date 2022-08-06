#pragma once

#include <random>

#include <QDebug>
#include <QObject>
#include <QVector3D>
#include <QWidget>

inline float randomF() { return static_cast<float>(rand()) / static_cast<float>(RAND_MAX); }

inline float randomS() { return (randomF() > .5f ? -1.f : 1.f); }

inline float randomB() { return (randomF() > .5f ? 0.f : 1.f); }

inline float randomN() { return randomS() * randomF(); }

inline QVector3D randomPointInUnitSphere()
{
    using namespace std;

    QVector3D p;
    do
    {
        p = 2.0f * QVector3D{randomF(), randomF(), randomF()} - QVector3D(1, 1, 1);
    } while (p.lengthSquared() >= 1.f);
    return p;
}

inline QVector3D reflect(const QVector3D & v, const QVector3D & n) noexcept
{
    return v - 2 * QVector3D::dotProduct(v, n) * n;
}

inline bool refract(const QVector3D & d, const QVector3D & n, float n1_over_n2, QVector3D & refracted)
{
    float dTn{QVector3D::dotProduct(d, n)};
    float discriminant{1 - n1_over_n2 * n1_over_n2 * (1 - dTn * dTn)};

    if (discriminant >= 0.0f)
    {
        refracted = n1_over_n2 * (d - n * dTn) - n * std::sqrt(discriminant);
        return true;
    }

    return false;
}

inline QVector3D randomPointInSquare()
{
    return 2.0f * QVector3D{randomF(), randomF(), 0} - QVector3D(1, 1, 0);
}

inline QVector3D randomPointInUnitDisk()
{
    QVector3D res;
    do
    {
        res = randomPointInSquare();
    } while (res.lengthSquared() > 1.f);
    return res;
}
