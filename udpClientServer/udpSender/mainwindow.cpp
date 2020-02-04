#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);

    threadForBackEnd = new QThread(this);
    backEnd = new BackEndWrapper("127.0.0.1",5557);
    backEnd->setDataContainer(&values);
    backEnd->moveToThread(threadForBackEnd);


    connect(this,&MainWindow::saveFileSignal,
            backEnd,&BackEndWrapper::saveFileSlot);

    connect(this,&MainWindow::loadFileSignal,
            backEnd,&BackEndWrapper::loadFileSlot);

    connect(backEnd,&BackEndWrapper::refreshWidgetSignal,
            this,&MainWindow::refreshWidgetSlot);

    connect(this,&MainWindow::generateDataSignal,
            backEnd,&BackEndWrapper::generateDataSlot);

    connect(threadForBackEnd, &QThread::finished,
            backEnd, &QObject::deleteLater);

    connect(this,&MainWindow::startTransfering,
            backEnd,&BackEndWrapper::startTransferingSlot);

    connect(backEnd,&BackEndWrapper::finishTransfering,
            this,&MainWindow::finishTransferingSlot);


    connect(threadForBackEnd,&QThread::started,
            backEnd,&BackEndWrapper::createSocket);

    threadForBackEnd->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    threadForBackEnd->quit();
    threadForBackEnd->wait();
}


void MainWindow::finishTransferingSlot()
{
    ui->centralWidget->setEnabled(true);
}


void MainWindow::open(){
    QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
        emit loadFileSignal(fileName);
}


void MainWindow::save(){
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return;
    emit saveFileSignal(dialog.selectedFiles().first());
}

void MainWindow::on_Gen_clicked()
{
    emit generateDataSignal();
}

void MainWindow::on_Send_clicked()
{
    ui->centralWidget->setEnabled(false);
    emit startTransfering();

}

void MainWindow::refreshWidgetSlot()
{
    ui->listWidgetNumbers->clear();
    QStringList items;
    /*This element added in the main thread, I can't do it faster,
     *so leave it as it is, Even stringListModel could not help.*/
    for(auto& item : values)
        items<<QString::number(item);

    ui->listWidgetNumbers->insertItems(0,items);
}
