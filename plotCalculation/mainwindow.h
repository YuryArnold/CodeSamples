#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QRubberBand>
#include <QtCharts/QChart>
#include <QRegularExpression>
#include <QThread>
#include <QLineEdit>
#include <functional>
#include <QtMath>
#include "sharedstructures.h"
#include "plot2d.h"
#include "functioncalculator.h"
#include "saveloaddata.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static void saveFunctionCallBack(void* _obj,QString& _path);
    static void loadFunctionCallBack(void* _obj,QString& _path);
    void saveFunction(QString& _path);
    void loadFunction(QString& _path);
    void enableWidgets(bool flag);
    void parseLoadedData();

private:
    bool checkForCorrectInput();
    void unionAllInputFields();
    void fillDataFromFields();
    void reinitializeFromLoad();

private slots:
    void on_buttonStart_clicked();
    void on_buttonPause_clicked();
    void on_buttonBreak_clicked();

signals:
    void calculateSignal();
    void signalForSave(QString);
    void signalForLoad(QString);
    void setFunctionParametrsSignal(FunctionParameters);

public slots:
    void setCalculatedPoint(double,double);
    void calculationProgressSlot(int);
    void setLoadedParametersSlot(FunctionParameters _parameters);
    void calculationAccomplishedSlot();

private:
    Ui::MainWindow *ui;
    QMap<QString,std::function<double(double&,FunctionParameters&)>> nameAndFunction;
    FunctionCalculator* functionCalculator = nullptr;
    QThread* threadForCalculator = nullptr;
    QThread* threadForSaveLoad = nullptr;
    plot2D* plot = nullptr;
    SaveLoadData* saveLoadObj = nullptr;
    FunctionParameters prm;
    QVector<double> points;
    QVector<QLineEdit*> inputFields;
    bool newPlot;
    bool calcAccomlished;
};

#endif // MAINWINDOW_H
