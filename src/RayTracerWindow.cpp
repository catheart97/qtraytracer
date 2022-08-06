#include "RayTracerWindow.h"

RayTracerWindow::RayTracerWindow(QWidget * parent) : QMainWindow(parent)
{
    /*
        Layout STUFF
    */
    auto frame = new QFrame(this);
    auto layout = new QGridLayout(frame);
    _widget = new RayTracerWidget(this);
    layout->addWidget(_widget, 0, 0, Qt::AlignCenter);
    setCentralWidget(frame);

    auto frame_right = new QFrame(this);
    frame_right->setMinimumWidth(300);
    frame_right->setMaximumWidth(300);

    auto right_layout = new QGridLayout(frame_right);

    _options_frame = new QFrame();
    auto right_upper_layout = new QGridLayout(_options_frame);
    right_layout->addWidget(_options_frame, 0, 0, Qt::AlignTop);
    auto label = new QLabel("Samples:");
    right_upper_layout->addWidget(label, 0, 0);

    _samples = new QSpinBox();
    _samples->setMinimum(1);
    _samples->setMaximum(500);
    _samples->setValue(_tracer.getSamples());
    right_upper_layout->addWidget(_samples, 1, 0);

    label = new QLabel("Dimension:");
    right_upper_layout->addWidget(label, 2, 0);
    _dimension = new QSpinBox();
    _dimension->setMinimum(256);
    _dimension->setMaximum(2048);
    _dimension->setValue(_tracer.getDimension());
    right_upper_layout->addWidget(_dimension, 3, 0);

    label = new QLabel("Tilesize:");
    right_upper_layout->addWidget(label, 4, 0);
    _tile_size = new QSpinBox();
    _tile_size->setMinimum(16);
    _tile_size->setMaximum(1024);
    _tile_size->setValue(_tracer.getTileSize());
    right_upper_layout->addWidget(_tile_size, 5, 0);

    _camera = std::make_shared<PerspectiveCamera>(QVector3D{0, 0, -4}, QVector3D{0, 0, 1}, QVector3D{0, 1, 0},
                                                  2.f, 1.f);

    label = new QLabel("Aperture:");
    right_upper_layout->addWidget(label, 6, 0);
    _aperture = new QDoubleSpinBox();
    _aperture->setMinimum(0.0);
    _aperture->setMaximum(100.0);
    _aperture->setValue(double(_camera->getAperture()));
    right_upper_layout->addWidget(_aperture, 7, 0);

    label = new QLabel("Focus Plane:");
    right_upper_layout->addWidget(label, 8, 0);
    _focus = new QDoubleSpinBox();
    _focus->setMinimum(1.0);
    _focus->setMaximum(20.0);
    _focus->setValue(double(_camera->getFocus()));
    right_upper_layout->addWidget(_focus, 9, 0);

    auto right_lower_layout = new QGridLayout();
    right_layout->addLayout(right_lower_layout, 1, 0, Qt::AlignBottom);

    auto calc_button = new QPushButton();
    calc_button->setText("Calculate.");
    connect(calc_button, &QPushButton::pressed, this, &RayTracerWindow::calculate);
    right_lower_layout->addWidget(calc_button, 0, 0);
    calc_button = new QPushButton();
    calc_button->setText("Stop calculation.");
    connect(calc_button, &QPushButton::pressed, this, &RayTracerWindow::stopCalculate);
    right_lower_layout->addWidget(calc_button, 1, 0);

    layout->addWidget(frame_right, 0, 1);

    auto fileMenu = new QMenu("&File", this);
    fileMenu->addAction("Save Image...", this, &RayTracerWindow::saveImage, Qt::CTRL | Qt::Key_O);
    menuBar()->addMenu(fileMenu);

    /*
        Initialize Scene
    */

    auto scene = std::make_shared<Scene>();

    auto diffuse_material = std::make_shared<DiffuseMaterial>(QVector3D{.8f, .6f, .6f});
    auto metal_material = std::make_shared<MetalMaterial>(QVector3D{.5f, .5f, .8f});
    auto dielectic_material = std::make_shared<DielectricMaterial>(1.3f);
    auto emission_material = std::make_shared<EmissionMaterial>();
    auto ground_material = std::make_shared<DiffuseMaterial>(QVector3D{.8f, .8f, .8f});

    std::shared_ptr<SphereHitable> spherehitable;
    std::shared_ptr<TriangleHitable> trianglehitable;

    //    trianglehitable = std::make_shared<TriangleHitable>(QVector3D{-.25f, -.25f, 0}, QVector3D{0, .25f,
    //    0},
    //                                                        QVector3D{.25f, -.25f, 0}); // center
    //    trianglehitable->setMaterial(dielectic_material);
    //    scene->addHitable(trianglehitable);

    std::vector<std::shared_ptr<SphereHitable>> spheres;

    spherehitable = std::make_shared<SphereHitable>(QVector3D(0, 0, 0), .25f); // center
    spherehitable->setMaterial(dielectic_material);
    scene->addHitable(spherehitable);
    spheres.push_back(spherehitable);

    spherehitable = std::make_shared<SphereHitable>(QVector3D(0, -100.25, 0), 100.f); // floor
    spherehitable->setMaterial(ground_material);
    scene->addHitable(spherehitable);
    spheres.push_back(spherehitable);

    spherehitable = std::make_shared<SphereHitable>(QVector3D(.4f, 0, 1), .25f); // left
    spherehitable->setMaterial(metal_material);
    scene->addHitable(spherehitable);
    spheres.push_back(spherehitable);

    spherehitable = std::make_shared<SphereHitable>(QVector3D(-.2f, -.15, -.5), .1f); // right
    spherehitable->setMaterial(diffuse_material);
    scene->addHitable(spherehitable);
    spheres.push_back(spherehitable);

    for (int i = 0; i < NUM_RANDOM_SPHERES; ++i)
    {
        bool collides{true};
        while (collides)
        {

            spherehitable = std::make_shared<SphereHitable>(QVector3D(randomN(), -0.2f, randomN()), .05f);

            collides = std::any_of(spheres.begin(), spheres.end(),
                                   [&spherehitable](std::shared_ptr<SphereHitable> sphere) {
                                       return sphere->hit(spherehitable);
                                   });
        }

        if (float r = randomF(); r > 0.25f)
        {
            if (r > .5f)
            {
                if (r > .6f)
                {
                    spherehitable->setMaterial(
                        std::make_shared<DiffuseMaterial>(QVector3D(randomF(), randomF(), randomF())));
                }
                else
                {
                    spherehitable->setMaterial(emission_material);
                }
            }
            else
            {
                spherehitable->setMaterial(std::make_shared<MetalMaterial>(
                    QVector3D(randomF(), randomF(), randomF()), std::fmod(randomF(), 0.2f)));
            }
        }
        else
        {
            spherehitable->setMaterial(dielectic_material);
        }

        scene->addHitable(spherehitable);
        spheres.push_back(spherehitable);
    }

    _tracer.setScene(scene);
    _tracer.setCamera(_camera);
    _tracer.addDrawable(_widget);

    _timer = std::make_unique<QTimer>();
    _timer->setInterval(500);
    connect(_timer.get(), &QTimer::timeout, this, &RayTracerWindow::updateFrame);

    calculate();
}

void RayTracerWindow::stopCalculate()
{
    _tracer.stopTracing();
    _options_frame->setEnabled(true);
}

void RayTracerWindow::saveImage()
{
    if (_tracer.isTracing())
    {
        QMessageBox::information(this, "Not allowed!", "The tracer is currently running!",
                                 QMessageBox::StandardButton::Ok);
        return;
    }

    auto image = _widget->getImage();

    QString filename =
        QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Portable Network Graphics (*.png)"));

    image.save(filename, "png");
}

void RayTracerWindow::calculate()
{

    if (!_tracer.isTracing())
    {
        _options_frame->setDisabled(true);

        _camera->setAperture(float(_aperture->value()));
        _camera->setFocusPlane(float(_focus->value()));
        _tracer.setDimension(_dimension->value());
        _tracer.setTileSize(_tile_size->value());
        _tracer.setSamples(_samples->value());

        _tracer.trace();

        _timer->start();
    }
}

void RayTracerWindow::updateFrame()
{
    int num_running = _tracer.getNumRunningThreads();

    if (num_running > 0)
    {
        QString message = "Calculating... Threads running " + QString::number(num_running) + " of " +
                          QString::number(_tracer.getTotalThreads()) + ".";
        statusBar()->showMessage(message);
    }
    else
    {
        statusBar()->showMessage("Finished.", 4000);
        stopCalculate();
    }

    update();
}
