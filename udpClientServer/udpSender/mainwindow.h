#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMenu>
#include <QSet>
#include "backendwrapper.h"
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void finishTransferingSlot();

private slots:
    void open();
    void save();
    void on_Gen_clicked();
    void on_Send_clicked();

public slots:
    void refreshWidgetSlot();

signals:
    void startTransfering();
    void loadFileSignal(QString);
    void saveFileSignal(QString);
    void generateDataSignal();

private:
    Ui::MainWindow *ui;
    QAction *openAct;
    QAction *saveAct;
    QMenu *fileMenu;
    QSet<int> values;
    BackEndWrapper* backEnd;
    QThread* threadForBackEnd;
};

#endif // MAINWINDOW_H
