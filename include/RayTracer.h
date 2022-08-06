#pragma once

#include <memory>
#include <random>
#include <vector>

#include <QDebug>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>

#include "Camera.h"
#include "Material.h"
#include "Ray.h"
#include "RayTracerDrawable.h"
#include "Scene.h"
#include "Utils.h"

struct Tile
{
    int _x0, _x1;
    int _y0, _y1;
    bool _processing; // alias processed
};

class RayTracer
{
private:
    // Scene data
    std::shared_ptr<Camera> _camera{nullptr};
    std::shared_ptr<Scene> _scene{nullptr};

    // Rendering drawer
    std::shared_ptr<QTimer> _timer;
    std::vector<RayTracerDrawable *> _drawables;

    // Rendering stuff
    int _num_samples{1};
    int _dimension{DIMENSION};
    int _tile_size{16};
    bool _tracing = false;
    QMutex _tile_mutex;
    std::vector<QFuture<void>> _threads;
    std::vector<std::shared_ptr<Tile>> _tiles;

private:
    void setPixel(int x, int y, const QColor & color);

public:
    bool isTracing() { return _tracing; }

    void stopTracing();

    void setSamples(int samples);

    void setDimension(int dimension);

    int getSamples() { return _num_samples; }

    int getDimension() { return _dimension; }

    int getTileSize() { return _tile_size; }

    void setTileSize(int tilesize)
    {
        if (!isTracing()) _tile_size = tilesize;
    }

    void addDrawable(RayTracerDrawable * drawable);

    void setCamera(std::shared_ptr<Camera> camera);

    void setScene(std::shared_ptr<Scene> scene) { _scene = scene; }

    QVector3D getColor(const Ray & r, int depth = 0) const;

    QVector3D background(const Ray & r) const;

    void trace(int x_0, int y_0, int x_1, int y_1);

    int getNumRunningThreads();

    size_t getTotalThreads() { return _threads.size(); }

    std::shared_ptr<Tile> getNextTile();

    void trace();
};
