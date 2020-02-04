#include "plotdata.h"
#include "ui_plotdata.h"
/*Use forward declaration and header after it!*/
#include "plot2d.h"
#include <QFileDialog>

PlotData::PlotData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotData)
{
    ui->setupUi(this);
    ui->tableDataFromPlot->setRowCount(1);
    ui->tableDataFromPlot->setColumnCount(2);
    ui->tableDataFromPlot->setHorizontalHeaderLabels(QStringList()<<tr("x")
                                                   <<tr("y"));
    ui->tableDataFromPlot->verticalHeader()->hide();
    ui->tableDataFromPlot->setEditTriggers(QTableWidget::NoEditTriggers);
}

PlotData::~PlotData()
{
    delete ui;
}


void PlotData::setPlotTableName(const QString &_name)
{
    ui->plotNameLabel->setText(_name);
}


void PlotData::append(double _x, double _y)
{
    int row  = ui->tableDataFromPlot->rowCount();
    ui->tableDataFromPlot->insertRow(row);
    ui->tableDataFromPlot->setItem(row,0,new QTableWidgetItem(QString::number(_x)));
    ui->tableDataFromPlot->setItem(row,1,new QTableWidgetItem(QString::number(_y)));
}

void PlotData::clear()
{
     ui->tableDataFromPlot->setRowCount(0);
}

void PlotData::on_loadData_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);
       if(fileName.isEmpty())
           return;

    plot2D* parentPtr = reinterpret_cast<plot2D*>(this->parent());
    parentPtr->loadData(fileName);
}

void PlotData::on_saveData_clicked()
{

    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return;

    plot2D* parentPtr = reinterpret_cast<plot2D*>(this->parent());
    parentPtr->saveData(dialog.selectedFiles().first());
}
