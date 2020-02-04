#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QObject>
#include <QPair>
#include <QThread>
#include <QDebug>
#include <QKeySequence>
#include <memory>
#include "crossitem.h"
#include "rawdatareader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setItemsToScene();

public slots:
    void getDataFromXml(QString pathToFile);
    void setParsedRaw(rawPointsContainer points);


signals:
    void readRawDataSignal(QString _pathToFile);

private slots:
    void setPathToXml();
    void setPathToRaw();

private:
    Ui::MainWindow *ui;
    QVector<std::shared_ptr<CrossItem>> data;
    QAction* openAct;
    QAction* openRawAct;
    QMenu *fileMenu;
    QThread* threadRawReader;
    RawDataReader* rawReader;
    rawPointsContainer checkPoints;
};

#endif // MAINWINDOW_H
