#include "plot2d.h"
#include <QDebug>
#include "mainwindow.h"

plot2D::plot2D(QWidget *parentWidget):
    QChartView(parentWidget)
{
    chartPlot = new Chart();
    plotTable = new PlotData(this);
    chartPlot->legend()->hide();
    chartPlot->setAnimationOptions(QChart::AllAnimations);
    setRenderHint(QPainter::Antialiasing);
    this->setChart(chartPlot);
    isClickable = false;
}

plot2D::~plot2D()
{
    delete chartPlot;
}


void plot2D::addPoint(double _x, double _y)
{
    argAndFunction.append(std::make_pair(_x,_y));
    chartPlot->addPoint(_x,_y);   
    plotTable->append(_x,_y);
}


void plot2D::clear()
{
   chartPlot->clear();
   plotTable->clear();
   argAndFunction.clear();
}

void plot2D::setCallBackSave(void (* _saveFuncPtr)(void*,QString&))
{
    callbackForSave = _saveFuncPtr;
}

void plot2D::setCallBackLoad(void (* _loadFuncPtr)(void*,QString&))
{
    callBackForLoad = _loadFuncPtr;
}

void plot2D::setCallableObject(void *_callableObject)
{
    callableObject = _callableObject;
}

vectorOfPoints *plot2D::getContainerPtr()
{
    return (&argAndFunction);
}

void plot2D::reset()
{
    chartPlot->clear();
    plotTable->clear();
    for(auto& item: argAndFunction){
        chartPlot->addPoint(item.first,item.second);
        plotTable->append(item.first,item.second);
    }
}

void plot2D::clickable(bool _state)
{
    isClickable = _state;
}


void plot2D::saveData(QString _path)
{
   callbackForSave(callableObject,_path);
}

void plot2D::loadData(QString _path)
{
    callBackForLoad(callableObject,_path);
}

void plot2D::setNameOfPlot(QString& _name)
{
    plotTable->setPlotTableName(_name);
}

void plot2D::mousePressEvent(QMouseEvent *event)
{
    if(!isClickable)
        return;

    if(event->button() == Qt::LeftButton)
    {
        plotTable->exec();
    }
    QChartView::mousePressEvent(event);
}


