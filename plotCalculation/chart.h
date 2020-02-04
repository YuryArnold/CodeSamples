#ifndef CHAT_H
#define CHAT_H
#include <QtCharts/QChart>
#include <QtCore/QTimer>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

class Chart: public QChart
{
    Q_OBJECT
public:
    Chart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~Chart();
    void setPoint(double _arg, double _func);
    void clear();

private:
    void rescaleAxis(double& _arg, double& _func);


public slots:
    void addPoint(double _arg, double _func);

private:
    QSplineSeries *series;
    QValueAxis *axis;
    double tmpMaxX;
    double tmpMinX;
    double tmpMaxY;
    double tmpMinY;
    double offset;

};
#endif // CHAT_H
