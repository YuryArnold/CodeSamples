#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QtMath>
#include <QtCharts/QLineSeries>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    newPlot = false;
    calcAccomlished = false;

    nameAndFunction.insert("f(x) = A*(x*x) + B*x + C",
                           [](double& x, FunctionParameters& params)->double{
                            return params.A * x*x + params.B*x + params.C;
                           });

    nameAndFunction.insert("f(x) = A * sin(x) + B * cos( C*x )",
                           [](double& x, FunctionParameters& params)->double{
                            return params.A * sin(x) + params.B*cos(x*params.C);
                           });


    nameAndFunction.insert("f(x) = A*log( B*x )",
                           [](double& x, FunctionParameters& params)->double{
                            return params.A *log(params.B*x);
                           });

    nameAndFunction.insert("f(x) = A / ( sin(x*x) * B )",
                           [](double& x, FunctionParameters& params)->double{
                            return params.A / (sin(x*x)*params.B);
                           });


    for(auto& item: nameAndFunction.keys())
        ui->functionBox->addItem(item);


    threadForCalculator = new QThread(this);
    functionCalculator = new FunctionCalculator;

    threadForSaveLoad = new QThread(this);
    saveLoadObj = new SaveLoadData;
    saveLoadObj->setPointerOfDataContainer(ui->plot->getContainerPtr());


    connect(threadForCalculator,&QThread::finished,
            threadForCalculator,&QThread::deleteLater);

    connect(threadForCalculator,&QThread::finished,
            functionCalculator,&FunctionCalculator::deleteLater);

    connect(threadForSaveLoad,&QThread::finished,
            threadForSaveLoad,&QThread::deleteLater);

    connect(threadForSaveLoad,&QThread::finished,
            saveLoadObj,&SaveLoadData::deleteLater);

    connect(functionCalculator,&FunctionCalculator::emitCalculatedPoint,
            this,&MainWindow::setCalculatedPoint);

    connect(this,&MainWindow::calculateSignal,
            functionCalculator,&FunctionCalculator::calculculateFunction);

    connect(this,&MainWindow::setFunctionParametrsSignal,
            functionCalculator,&FunctionCalculator::setFunctionParametersSlot);

    connect(functionCalculator,&FunctionCalculator::calculationProgress,
            this,&MainWindow::calculationProgressSlot);

    connect(functionCalculator,&FunctionCalculator::calculationAccomplished,
            this,&MainWindow::calculationAccomplishedSlot);

    connect(this,&MainWindow::signalForSave,
            saveLoadObj,&SaveLoadData::writeDataToFile);

    connect(this,&MainWindow::signalForLoad,
            saveLoadObj,&SaveLoadData::readDataFromFile);

    connect(saveLoadObj,&SaveLoadData::getLoadedParametersSignal,
            this,&MainWindow::setLoadedParametersSlot);


    /*Ubpdate Data for saving for every state of pause or break!*/
    connect(functionCalculator,&FunctionCalculator::setLoadedParametrs,
            saveLoadObj,&SaveLoadData::setLoadedParametersSlot);


    unionAllInputFields();
    plot = ui->plot;
    plot->setCallableObject(this);
    plot->setCallBackLoad(&MainWindow::loadFunctionCallBack);
    plot->setCallBackSave(&MainWindow::saveFunctionCallBack);
    functionCalculator->moveToThread(threadForCalculator);
    threadForCalculator->start();
    saveLoadObj->moveToThread(threadForSaveLoad);
    threadForSaveLoad->start();

}

MainWindow::~MainWindow()
{
    functionCalculator->pauseCalculation(true);
    threadForCalculator->quit();
    threadForCalculator->wait();
    threadForSaveLoad->quit();
    threadForSaveLoad->wait();
    delete plot;
    delete ui;
}

void MainWindow::saveFunctionCallBack(void *_obj,QString& _path)
{
    MainWindow * self = static_cast<MainWindow*>(_obj);
       self->saveFunction(_path);
}

void MainWindow::loadFunctionCallBack(void *_obj,QString& _path)
{
    MainWindow * self = static_cast<MainWindow*>(_obj);
       self->loadFunction(_path);
}

void MainWindow::saveFunction(QString& _path)
{
    emit signalForSave(_path);
}

void MainWindow::loadFunction(QString& _path)
{
    emit signalForLoad(_path);
}

void MainWindow::enableWidgets(bool flag)
{

    for(auto& item: inputFields){
        item->setEnabled(flag);
    }

    ui->functionBox->setEnabled(flag);

}

void MainWindow::parseLoadedData()
{
    switch (prm.state){
    case pause:
        newPlot = false;
        functionCalculator->reset();
        functionCalculator->pauseCalculation(true);
        ui->buttonPause->setText("Continue");
        plot->clickable(true);
        break;

    case over:
        on_buttonBreak_clicked();
        break;
    }
}

void MainWindow::on_buttonStart_clicked()
{
    if(functionCalculator->isPaused())
        return;

    if(newPlot) {
        newPlot = false;
        functionCalculator->reset();
    }

    if(!checkForCorrectInput()){
        return;
    } else {
           plot->clear();
           plot->clickable(false);
           enableWidgets(false);
           QString key = ui->functionBox->currentText();
           functionCalculator->setFunction(nameAndFunction[key]);
           plot->setNameOfPlot(key);
           prm.function = key;
           emit setFunctionParametrsSignal(prm);
           QThread::msleep(10);
           emit calculateSignal();
    }
}

void MainWindow::on_buttonPause_clicked()
{
    if(newPlot)
        return;

    if(!functionCalculator->isPaused()){
        functionCalculator->pauseCalculation(true);
        ui->buttonPause->setText("Continue");
        plot->clickable(true);
    } else {
       functionCalculator->pauseCalculation(false);
       ui->buttonPause->setText("Pause");
       plot->clickable(false);
       emit calculateSignal();
    }

}

void MainWindow::on_buttonBreak_clicked()
{
    functionCalculator->setBreak();
    ui->buttonStart->setText("New");
    ui->buttonPause->setText("Pause");
    enableWidgets(true);
    plot->clickable(true);
    newPlot = true;
}

void MainWindow::setCalculatedPoint(double _arg, double _func)
{
    plot->addPoint(_arg,_func);
}

void MainWindow::calculationProgressSlot(int progress)
{
    ui->buttonStart->setText("Progress % " + QString::number(progress));
}


/*Return to This function and implemnt function of resetting function!*/
void MainWindow::setLoadedParametersSlot(FunctionParameters _parameters)
{
    /*Set Loaded Function Data!*/
    prm = _parameters;
    int index = ui->functionBox->findText(prm.function);
    if(index < 0){
        QMessageBox::critical(this,tr("Error in File!"),tr("Incorrect File was opened!\nTry another."));
        return;
    }
    ui->functionBox->setCurrentIndex(index);
    functionCalculator->setFunction(nameAndFunction[prm.function]);
    plot->setNameOfPlot(prm.function);
    enableWidgets(false);
    plot->reset();
    reinitializeFromLoad();
    parseLoadedData();
    emit setFunctionParametrsSignal(prm);
}

void MainWindow::calculationAccomplishedSlot()
{
    on_buttonBreak_clicked();
}

bool MainWindow::checkForCorrectInput()
{
   QRegularExpression regExp("^(-?)(0|([1-9][0-9]*))(\\.[0-9]+)?$");
   for(auto& fieldItem: inputFields){

       QRegularExpressionMatch match = regExp.match(fieldItem->text());
       if(!match.hasMatch()){
           QMessageBox::warning(this,tr("Input Error"),tr("Check for correct input\nit has erorrs!"));
           return false;
       }
   }

   //Check for correct interval
   prm.start = ui->lineEditFrom->text().toDouble();
   prm.stop = ui->lineEditTo->text().toDouble();
   if((prm.stop - prm.start) < 0){
       QMessageBox::warning(this,tr("Input Error"),tr("Incorect calculation Interval!"));
       return false;
   }
   prm.step = ui->lineEditStep->text().toDouble();
   if(prm.step < 0){
       QMessageBox::warning(this,tr("Input Error"),tr("Step Must Be positive value!"));
       return false;
   }

   prm.startInd = 0;
   fillDataFromFields();
   return true;
}

void MainWindow::unionAllInputFields()
{
    inputFields.append(ui->lineEditA);
    inputFields.append(ui->lineEditB);
    inputFields.append(ui->lineEditC);
    inputFields.append(ui->lineEditFrom);
    inputFields.append(ui->lineEditTo);
    inputFields.append(ui->lineEditStep);
}

void MainWindow::fillDataFromFields()
{
    prm.A = ui->lineEditA->text().toDouble();
    prm.B = ui->lineEditB->text().toDouble();
    prm.C = ui->lineEditC->text().toDouble();
    prm.step = ui->lineEditStep->text().toDouble();
}

void MainWindow::reinitializeFromLoad()
{
    ui->lineEditA->setText(QString::number(prm.A));
    ui->lineEditB->setText(QString::number(prm.B));
    ui->lineEditC->setText(QString::number(prm.C));
    ui->lineEditFrom->setText(QString::number(prm.start));
    ui->lineEditTo->setText(QString::number(prm.stop));
    ui->lineEditStep->setText(QString::number(prm.step));
}
