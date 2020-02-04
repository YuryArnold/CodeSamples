#ifndef PLOTDATA_H
#define PLOTDATA_H
#include <QDialog>
#include <QVector>
#include <map>

namespace Ui {
class PlotData;
}

class PlotData : public QDialog
{
    Q_OBJECT

public:
    explicit PlotData(QWidget *parent = 0);
    ~PlotData();
    void setPlotTableName( const QString& _name);
    void append(double _x,double _y);
    void clear();

private slots:
    void on_loadData_clicked();
    void on_saveData_clicked();

private:
    Ui::PlotData *ui;

};

#endif // PLOTDATA_H
