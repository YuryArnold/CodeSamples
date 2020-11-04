#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QXmlStreamReader>
#include <QMenuBar>
#include <QTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*Initialize random generator*/
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    states = QVector<QVector<bool>>(numbOfElem,QVector<bool>(numbOfElem));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Open);
    saveAct->setStatusTip(tr("Open an existing file"));
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(saveAct);
    connect(saveAct, &QAction::triggered,
            this, &MainWindow::saveToDb);


    downloadAct = new QAction(tr("&Download"), this);
    downloadAct->setShortcut(QKeySequence::New);
    downloadAct->setStatusTip(tr("Download file if exists"));
    fileMenu->addAction(downloadAct);
    connect(downloadAct, &QAction::triggered,
            this, &MainWindow::downloadFromDb);

    generateAction = new QAction(tr("&Generate"),this);
    generateAction->setStatusTip(tr("Generate new maze"));
    fileMenu->addAction(generateAction);
    connect(generateAction, &QAction::triggered,
            this, &MainWindow::generateMaze);


    pathFinderThread = new QThread;
    pathFinder = new BfsFinder(states);
    pathFinder->moveToThread(pathFinderThread);
    pathFinderThread->start();

    auto scene = qobject_cast<GraphicsScene*>(ui->sceneOfSquares->scene());

    connect(scene,&GraphicsScene::calculatePathSignal,
            pathFinder,&BfsFinder::BFSCalculate);


    connect(pathFinder,&BfsFinder::setCalculatedPath,
            this,&MainWindow::setMazePath);


    connect(pathFinder,&BfsFinder::noPathSignal,
            this,&MainWindow::noPathSlot);


}

MainWindow::~MainWindow()
{
    delete ui;
    pathFinderThread->quit();
    pathFinderThread->wait();
    delete pathFinderThread;
    delete pathFinder;
}


void MainWindow::setMazePath(PointsContainer points)
{
    if (!duringPoints.isEmpty()){
        for(auto& pt : duringPoints)
            items[pt.first * (numbOfElem)  + pt.second]->setState(State::Empty);
    }

    duringPoints = points;
    //Make start and end points during calculation
    auto tail = points.takeFirst();
    auto head = points.takeLast();

    items[head.first * (numbOfElem)  + head.second]->setState(State::Start);
    items[tail.first * (numbOfElem)  + tail.second]->setState(State::End);

    for(auto& pt : points){
        items[pt.first * (numbOfElem)  + pt.second]->setState(State::Path);
    }
}

void MainWindow::noPathSlot()
{
    QMessageBox msgBox;
    msgBox.setText("There is no path between points!");
    msgBox.exec();
}

void MainWindow::saveToDb()
{

}

void MainWindow::downloadFromDb()
{

}

void MainWindow::generateMaze()
{

    ui->sceneOfSquares->clear();
    for(int row = 0; row < numbOfElem; ++row)
        for(int col = 0; col < numbOfElem; ++col)
            states[row][col] = isBlack();

    duringPoints.clear();
    items.clear();
    createMaze(states);
    pathFinder->setStateArray(states);

    /*set elements to scene!*/
    for(const auto& square : items){
        ui->sceneOfSquares->addItem(square);
    }

}

void MainWindow::createMaze(QVector<QVector<bool> > &_states)
{
    float itemWidth = sceneWidth / numbOfElem;
    float itemHeigh = sceneHeight / numbOfElem;

    for(int row = -numbOfElem/2; row < numbOfElem/2; ++row){
        QVector<SquareItem*> tmp;
        for(int col =  -numbOfElem/2; col < numbOfElem/2; ++col){
            auto ptr = new SquareItem();
            ptr->setRect(QRectF((col * itemWidth) + 2,
                                (row * itemHeigh) + 2,
                                itemWidth,
                                itemHeigh));
            ptr->setGridCoord(row + numbOfElem/2, col + numbOfElem/2);
            if(!_states[row + numbOfElem/2][col + numbOfElem/2])
                ptr->setState(State::Colored);
            tmp.append(ptr);
        }
        items.append(std::move(tmp));
    }
}

bool MainWindow::isBlack()
{
   return (qrand()%100) > 50;
}


