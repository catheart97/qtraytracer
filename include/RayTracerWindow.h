#pragma once

#include <cmath>
#include <memory>

#include <QCoreApplication>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMutex>
#include <QPushButton>
#include <QSpinBox>
#include <QStatusBar>
#include <QtWidgets/QMainWindow>

#include "DielectricMaterial.h"
#include "DiffuseMaterial.h"
#include "EmissionMaterial.h"
#include "MetalMaterial.h"
#include "PerspectiveCamera.h"
#include "Ray.h"
#include "RayTracer.h"
#include "RayTracerWidget.h"
#include "Scene.h"
#include "SphereHitable.h"
#include "TriangleHitable.h"

/*

TODO:
- Clear image at calculation start
- Thread Tiling

*/

class RayTracerWindow : public QMainWindow
{
    Q_OBJECT

private:
    QFrame * _options_frame;
    QSpinBox *_samples, *_dimension, *_tile_size;
    QDoubleSpinBox *_aperture, *_focus;

    RayTracerWidget * _widget{nullptr};
    RayTracer _tracer;
    std::shared_ptr<PerspectiveCamera> _camera;

    std::unique_ptr<QTimer> _timer;

public:
    RayTracerWindow(QWidget * parent = nullptr);

private slots:
    void calculate();

    void stopCalculate();

public slots:
    void saveImage();

    void updateFrame();

protected:
    void closeEvent(QCloseEvent * event) override
    {
        stopCalculate();
        event->accept();
    }
};
