#include "mainwindow.h"
#include <QApplication>
#include "typedefs.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<PointsContainer>("PointsContainer");
    qRegisterMetaType<Point>("Point");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
