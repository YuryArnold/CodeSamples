#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    connectWasClicked = false;
    // I put
    connect(ui->BaudRateBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this, &MainWindow::checkCustomBaudRatePolicy);
    ui->BaudRateBox->addItem(QLatin1String("9600"), QSerialPort::Baud9600);
    ui->BaudRateBox->addItem(QLatin1String("19200"), QSerialPort::Baud19200);
    ui->BaudRateBox->addItem(QLatin1String("38400"), QSerialPort::Baud38400);
    ui->BaudRateBox->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
    ui->BaudRateBox->addItem(QLatin1String("Custom"));
   // fill data bits
    ui->DataBitsBox->addItem(QLatin1String("5"), QSerialPort::Data5);
    ui->DataBitsBox->addItem(QLatin1String("6"), QSerialPort::Data6);
    ui->DataBitsBox->addItem(QLatin1String("7"), QSerialPort::Data7);
    ui->DataBitsBox->addItem(QLatin1String("8"), QSerialPort::Data8);
    ui->DataBitsBox->setCurrentIndex(3);
   // fill parity
    ui->ParityBox->addItem(QLatin1String("None"), QSerialPort::NoParity);
    ui->ParityBox->addItem(QLatin1String("Even"), QSerialPort::EvenParity);
    ui->ParityBox->addItem(QLatin1String("Odd"), QSerialPort::OddParity);
    ui->ParityBox->addItem(QLatin1String("Mark"), QSerialPort::MarkParity);
    ui->ParityBox->addItem(QLatin1String("Space"), QSerialPort::SpaceParity);
   // fill stop bits
    ui->StopBitsBox->addItem(QLatin1String("1"), QSerialPort::OneStop);
    ui->StopBitsBox->addItem(QLatin1String("2"), QSerialPort::TwoStop);
   // fill flow control
    ui->FlowControlBox->addItem(QLatin1String("None"), QSerialPort::NoFlowControl);
    ui->FlowControlBox->addItem(QLatin1String("RTS/CTS"), QSerialPort::HardwareControl);
    ui->FlowControlBox->addItem(QLatin1String("XON/XOFF"), QSerialPort::SoftwareControl);

    //Connecting Sender and Serializer for it's own threads.
    threadForSerializer = new QThread(this);
    serializer = new Serializer;

    threadForSender = new QThread(this);
    sender = new Sender;
    sender->setSerializerObject(serializer);

    smartBuffer = new SmartBuffer(this);

    sender->setSmartBuffer(smartBuffer);
    serializer->setSmartBuffer(smartBuffer);

    serializer->moveToThread(threadForSerializer);
    sender->moveToThread(threadForSender);

    ui->sendProgress->reset();

    connect(threadForSerializer, &QThread::finished,
                serializer, &QObject::deleteLater);

    connect(threadForSender, &QThread::finished,
                sender, &QObject::deleteLater);

    connect(this,&MainWindow::connectSignal,
            sender,&Sender::connectPort);

    connect(this,&MainWindow::disconnectSignal,
            sender,&Sender::disconnectPort);

    connect(sender,&Sender::stringMessage,
            this, &MainWindow::stringMessageSlot);

    connect(sender,&Sender::connectPortSignal,
            this,&MainWindow::slotForConnectionState);

    connect(this,&MainWindow::setPathToSendedFileSignal,
            serializer,&Serializer::setPathToSendedFileSlot);

    connect(this,&MainWindow::startSending,
            serializer,&Serializer::burstLoop);

    connect(this,&MainWindow::disconnectSignal,
            serializer,&Serializer::closeFileWithPort);

    connect(sender,&Sender::transferAccomplished,
            this,[&](){

        ui->resultEdit->textCursor().insertText(QString("Transfer Accomplished")+'\r');
        ui->resultEdit->moveCursor(QTextCursor::End);
    });


    /*Do not forget set minimum and maximum to progress bar.*/
    connect(sender,&Sender::duringSendProgress,
            ui->sendProgress,&QProgressBar::setValue);

    threadForSerializer->start();
    threadForSender->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(sender->isOpen())
        emit disconnectSignal();
    threadForSender->quit();
    threadForSender->wait();

    threadForSerializer->quit();
    threadForSerializer->wait();
}

void MainWindow::on_Btn_Serch_clicked()
{
    ui->PortNameBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        ui->PortNameBox->addItem(info.portName());
    }
}

void MainWindow::on_BtnConnect_clicked()
{
    if(!connectWasClicked){
        connectWasClicked = true;
        Settings portSettings;
        portSettings.name = ui->PortNameBox->currentText();
        portSettings.baudRate    = (QSerialPort::BaudRate) ui->BaudRateBox->currentText().toInt();
        portSettings.dataBits    = (QSerialPort::DataBits) ui->DataBitsBox->currentText().toInt();
        portSettings.flowControl = (QSerialPort::FlowControl) ui->FlowControlBox->currentText().toInt();
        portSettings.parity      = (QSerialPort::Parity)ui->ParityBox->currentText().toInt();
        portSettings.stopBits    = (QSerialPort::StopBits)ui->StopBitsBox->currentText().toInt();
        sender->setSettings(portSettings);
        emit connectSignal();
    } else {
        connectWasClicked = false;
        emit disconnectSignal();
    }
}

void MainWindow::stringMessageSlot(QString _message)
{
    ui->connectInformationEdit->textCursor().insertText(_message+'\r');
    ui->connectInformationEdit->moveCursor(QTextCursor::End);
}



void MainWindow::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->BaudRateBox->itemData(idx).isValid();
    ui->BaudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate)
        ui->BaudRateBox->clearEditText();
}

void MainWindow::slotForConnectionState(bool _connectState)
{
    connectWasClicked = _connectState;
    connectWasClicked?(ui->BtnConnect->setText("Disconnect"))
                    :(ui->BtnConnect->setText("Connect"));
    if(!connectWasClicked){
        ui->resultEdit->clear();
        ui->sendProgress->reset();
    }
}


void MainWindow::on_sendFileButton_clicked()
{
    smartBuffer->reset();
    ui->sendProgress->reset();
    emit startSending();
}

void MainWindow::on_selectFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);
       if(!fileName.isEmpty())
           emit setPathToSendedFileSignal(fileName);
}
