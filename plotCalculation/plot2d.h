#ifndef PLOT2D_H
#define PLOT2D_H
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QAbstractAxis>
#include <QDialog>
#include <QString>
#include "sharedstructures.h"
#include "plotdata.h"
#include "chart.h"

class MainWindow;

QT_CHARTS_USE_NAMESPACE

class plot2D : public QChartView
{
public:
    explicit plot2D(QWidget *parentWidget = nullptr);
    virtual ~plot2D();
    void addPoint(double _x,double _y);
    void saveData(QString);
    void loadData(QString);
    void setNameOfPlot(QString&);
    void clear();
    /*This 2 functions instead signal/slot connection,
    I can't avoid diamond inheritance, because of
    it I used callback technicue. */
    void setCallBackSave(void (* _saveFuncPtr) (void*,QString&));
    void setCallBackLoad(void (* _loadFuncPtr) (void*,QString&));
    bool canDisplayInputData();
    void setClassForCallBack(MainWindow* _parent);
    void setCallableObject(void* _callableObject);
    vectorOfPoints* getContainerPtr();
    void reset();
    void clickable(bool _state);

protected:
    void mousePressEvent(QMouseEvent* event);

signals:
    void saveDataSignal();

private:
    bool isClickable;
    Chart* chartPlot = nullptr;
    PlotData* plotTable = nullptr;
    void (*callbackForSave)(void*,QString&);
    void (*callBackForLoad)(void*,QString&);
    vectorOfPoints argAndFunction;
    bool displayInputData;
    void* callableObject;
};


#endif // PLOT2D_H
