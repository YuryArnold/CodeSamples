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
#include "variables.h"
#include "squareitem.h"
#include "bfsfinder.h"
#include "typedefs.h"
#include "dbmanager.h"
#include "snakelogick.h"

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
    void setMazePath(PointsContainer points);
    void noPathSlot();

signals:
    void readRawDataSignal(QString _pathToFile);

private slots:
    void saveToDb();
    void downloadFromDb();
    void generateMaze();

private:
    void createMaze(QVector<QVector<bool>>& _states);
    bool isBlack();
    void generateRandomMaze();
    void generateSnakes();
    Ui::MainWindow *ui;
    QAction* saveAct;
    QAction* downloadAct;
    QAction* generateAction;
    QMenu *fileMenu;
    QThread* pathFinderThread;
    QVector<SquareItem*> items;
    BfsFinder* pathFinder;
    SnakeLogick snakeLogick;
    PointsContainer duringPoints;
    QVector<QVector<bool>> states;

};

#endif // MAINWINDOW_H
