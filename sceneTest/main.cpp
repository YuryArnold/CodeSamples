#include "mainwindow.h"
#include <QApplication>
#include "typedefs.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<rawPointsContainer>();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
