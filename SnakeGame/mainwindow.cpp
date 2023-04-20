#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(320,380);
    m_snakeFieldPtr = new SnakeField();
    m_scoreFieldPtr = new ScoreField();
    m_centralWidgetPtr = new QWidget();

    connect(m_snakeFieldPtr,
            &SnakeField::changeTextSignal,
            m_scoreFieldPtr,
            &ScoreField::onChangeTextSlot);

    m_gridLayoutPtr = new QGridLayout();
    m_gridLayoutPtr->addWidget(m_snakeFieldPtr,0,0);
    m_gridLayoutPtr->addWidget(m_scoreFieldPtr,1,0);
    m_centralWidgetPtr->setLayout(m_gridLayoutPtr);
    setCentralWidget(m_centralWidgetPtr);
    setStyleSheet("background: rgb(90,100,112);");
}

MainWindow::~MainWindow()
{
    delete ui;
}

