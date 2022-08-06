#include <chrono>
#include <random>

#include <QStyleFactory>
#include <QtWidgets/QApplication>

#include "RayTracerWindow.h"

int main(int argc, char * argv[])
{
    QApplication a(argc, argv);

    std::srand(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

    //srand(23);

    a.setStyle(QStyleFactory().create("Fusion"));

    RayTracerWindow w;
    w.showMaximized();

    return a.exec();
}
