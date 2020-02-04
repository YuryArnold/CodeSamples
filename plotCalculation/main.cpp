#include "mainwindow.h"
#include <QApplication>
#include "sharedstructures.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<FunctionParameters>();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
