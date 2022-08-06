#include "RayTracer.h"

void RayTracer::setPixel(int x, int y, const QColor & color)
{
    for (auto drawable : _drawables) drawable->setPixel(x, y, color);
}

void RayTracer::stopTracing()
{
    _tracing = false;
    for (auto task : _threads)
    {
        task.waitForFinished();
    }
}

void RayTracer::setSamples(int samples)
{
    if (!isTracing())
    {
        _num_samples = samples;
    }
}

void RayTracer::setDimension(int dimension)
{
    if (!_tracing)
    {
        _dimension = dimension;
        _camera->setPixelDimension(dimension);
        for (auto draw : _drawables) draw->updateSize(dimension);
    }
}

void RayTracer::addDrawable(RayTracerDrawable * drawable)
{
    _drawables.push_back(drawable);
    setDimension(getDimension());
}

void RayTracer::setCamera(std::shared_ptr<Camera> camera)
{
    _camera = camera;
    setDimension(getDimension());
}

QVector3D RayTracer::getColor(const Ray & ray, int depth) const
{
    HitRecord rec;
    if (depth < BOUNCE_DEPTH && _scene && _scene->hit(ray, 0.0001f, std::numeric_limits<float>::max(), rec))
    {
        std::shared_ptr<Material> material = rec._hit->getMaterial();
        if (material)
            return material->getColor(ray, rec, this, depth + 1);
        else
            return {0, 0, 0};
    }
    else
    {
        return background(ray);
    }
}

QVector3D RayTracer::background(const Ray & r) const
{
    // beautiful blue sky background
    auto dir = r.direction().normalized();
    float t = .5f * (dir.y() + 1.f);
    return {((1.f - t) * QVector3D(1, 1, 1) + t * QVector3D(222, 211, 166) / 255.)};
}

void RayTracer::trace(int x_0, int y_0, int x_1, int y_1)
{
    if (!_camera) return;

    for (auto x = x_0; x < x_1; ++x)
    {
        for (auto y = y_0; y < y_1; ++y)
        {
            if (!_tracing) return; // stop thread execution

            QVector3D color;

            for (auto s = 0; s < _num_samples; ++s)
                color += getColor(_camera->getRay(x + randomF(), y + randomF())); // random sampling
            color /= _num_samples;

            color = {std::sqrt(color[0]), std::sqrt(color[1]), std::sqrt(color[2])}; // gamma correction

            color *= 255.f;
            setPixel(x, _dimension - y - 1, QColor(int(color[0]), int(color[1]), int(color[2])));
        }
    }
}

int RayTracer::getNumRunningThreads()
{
    return static_cast<int>(std::count_if(_threads.begin(), _threads.end(),
                                          [](const QFuture<void> & val) { return val.isRunning(); }));
}

std::shared_ptr<Tile> RayTracer::getNextTile()
{
    QMutexLocker ml(&_tile_mutex);

    for (size_t i = 0; i < _tiles.size(); ++i)
    {
        auto ptr = _tiles[i];
        if (!ptr->_processing)
        {
            ptr->_processing = true;
            return ptr;
        }
    }
    return nullptr;
}

void RayTracer::trace()
{
    if (_tracing) return;

    _tracing = true;

    // Generate tiles depending on tile_size
    _tiles.clear();
    int tiles_dim = int(std::ceil(_dimension / float(_tile_size)));
    for (int x = 0; x < tiles_dim; ++x)
    {
        for (int y = 0; y < tiles_dim; ++y)
        {
            auto tile = std::make_shared<Tile>();
            tile->_x0 = x * _tile_size;
            tile->_x1 = std::min((x + 1) * _tile_size, _dimension);
            tile->_y0 = y * _tile_size;
            tile->_y1 = std::min((y + 1) * _tile_size, _dimension);
            _tiles.push_back(std::move(tile));
        }
    }

    _threads.resize(size_t(QThread::idealThreadCount()));

    for (size_t i = 0; i < _threads.size(); ++i)
    {
        _threads[i] = QtConcurrent::run([this]() {
            std::shared_ptr<Tile> tile = getNextTile();
            while (tile && isTracing())
            {
                trace(tile->_x0, tile->_y0, tile->_x1, tile->_y1);
                tile = getNextTile();
            }
        });
    }
}
