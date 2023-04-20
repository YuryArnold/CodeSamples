#pragma once

#include <QMainWindow>
#include <QGridLayout>
#include "snakegame.h"
#include "scorefield.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget* m_centralWidgetPtr = nullptr;
    SnakeField* m_snakeFieldPtr = nullptr;
    ScoreField* m_scoreFieldPtr = nullptr;
    QGridLayout* m_gridLayoutPtr = nullptr;
};

