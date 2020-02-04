#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QXmlStreamReader>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include "crossitem.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered,
            this, &MainWindow::setPathToXml);


    openRawAct = new QAction(tr("&Open raw"), this);
    openRawAct->setShortcut(QKeySequence::New);
    openRawAct->setStatusTip(tr("Open raw file"));
    fileMenu->addAction(openRawAct);
    connect(openRawAct, &QAction::triggered,
            this, &MainWindow::setPathToRaw);


    threadRawReader = new QThread;
    rawReader = new RawDataReader;
    rawReader->moveToThread(threadRawReader);
    threadRawReader->start();

    connect(this, &MainWindow::readRawDataSignal,
            rawReader,&RawDataReader::setPathToFile);

    connect(rawReader,&RawDataReader::parsedData,
            this,&MainWindow::setParsedRaw);

}

MainWindow::~MainWindow()
{
    delete ui;
    threadRawReader->quit();
    threadRawReader->wait();
    delete rawReader;
    delete threadRawReader;


}

void MainWindow::setItemsToScene()
{
    for(const auto& cross : data){
        ui->sceneOfCross->addItem(cross.get());
    }
}

void MainWindow::getDataFromXml(QString _pathToFile)
{
    data.clear();
    ui->sceneOfCross->Clear();
    QFile file(_pathToFile);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(nullptr,tr("Udp Sender."),tr("Error in load file\nTry another directory/"));
        file.close();
        return;
    }

    QXmlStreamReader xmlDoc(&file);

    while(!xmlDoc.atEnd() && !xmlDoc.hasError()){
        QXmlStreamReader::TokenType token = xmlDoc.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        if(token == QXmlStreamReader::StartElement) {
            if(xmlDoc.name() == "Caption")
                continue;
            if(xmlDoc.name() == "Point"){
                QXmlStreamAttributes attributes = xmlDoc.attributes();
                QString xStr = attributes.value("X").toString();
                QString yStr = attributes.value("Y").toString();
                auto item = std::make_shared<CrossItem>(xStr.toDouble(),yStr.toDouble());
                data.push_back(item);
            }

        }
    }
    file.close();
    setItemsToScene();
}


/**
 * @brief MainWindow::setParsedRaw
 * @param points
 */
void MainWindow::setParsedRaw(rawPointsContainer points)
{
    qDebug()<<"You get parsed points";
    checkPoints.append(points);
}

void MainWindow::setPathToXml()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
        getDataFromXml(fileName);
}

void MainWindow::setPathToRaw()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
        emit readRawDataSignal(fileName);;

}


