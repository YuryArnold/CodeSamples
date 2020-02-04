#include "chart.h"
#include <QtCharts/QAbstractAxis>
#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    series(nullptr),
    axis(new QValueAxis)
{
    series = new QSplineSeries(this);
    QPen red(Qt::red);
    red.setWidth(3);
    series->setPen(red);
    addSeries(series);
    createDefaultAxes();
    setAxisX(axis, series);
    axis->setTickCount(5);
    axisX()->setRange(-1, 3);
    axisY()->setRange(-2, 2);
    offset = 4;
}

Chart::~Chart()
{

}

void Chart::clear()
{
   series->clear();
   tmpMinX = 0;
   tmpMaxX = 0;
   tmpMaxY = 0;
   tmpMinY = 0;

}

void Chart::rescaleAxis(double &_arg, double &_func)
{

    if(_arg < tmpMinX)
        tmpMinX = _arg;
    if(_arg > tmpMaxX)
        tmpMaxX = _arg;
    if(_func > tmpMaxY)
        tmpMaxY = _func;
    if(_func < tmpMinY)
        tmpMinY = _func;

    axisX()->setRange(tmpMinX - offset, tmpMaxX + offset);
    axisY()->setRange(tmpMinY - offset, tmpMaxY+ offset);
}



void Chart::addPoint(double _arg, double _func)
{
    rescaleAxis(_arg,_func);
    series->append(_arg,_func);
}
