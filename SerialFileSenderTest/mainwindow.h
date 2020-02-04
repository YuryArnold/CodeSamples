#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "serializer.h"
#include "sender.h"
#include "settingsstructure.h"
#include "smartbuffer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Btn_Serch_clicked();
    void checkCustomBaudRatePolicy(int idx);
    void slotForConnectionState(bool);
    void on_BtnConnect_clicked();
    void stringMessageSlot(QString);
    void on_sendFileButton_clicked();
    void on_selectFileButton_clicked();

signals:
    void connectSignal();
    void disconnectSignal();
    void setPathToSendedFileSignal(QString _path);
    void sendFileSignal();
    void startSending();

private:
    bool connectWasClicked;
    Ui::MainWindow *ui;
    QThread* threadForSender;
    QThread* threadForSerializer;
    Sender* sender;
    Serializer* serializer;
    SmartBuffer* smartBuffer;
};

#endif // MAINWINDOW_H
