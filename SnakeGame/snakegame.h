#pragma once

#include <QWidget>
#include <QPainter>
#include <QPair>
#include <QList>
#include <QTimer>
#include <QKeyEvent>
#include <QRandomGenerator>

struct SnakeItem;
struct Snake;
class SnakeField;


struct SnakeItem
{
    SnakeItem(int _x, int _y);
    int m_X;
    int m_Y;
};

struct Snake
{
    Snake();
    QList<SnakeItem*> m_snakeBody;
    int m_snakeStartSize;

    enum SnakeDirection {
        up,
        left,
        right,
        down
    };

    SnakeDirection m_snakeDirection;
};


class SnakeField : public QWidget
{
    Q_OBJECT
public:
    SnakeField();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void changeTextSignal(QString _text);

private:
   Snake* m_snakePtr= nullptr;
   int m_snakeItemSize;
   QTimer* m_moveSnakeTimerPtr = nullptr;
   int m_fieldSize;
   bool m_isPause;
   bool m_isMoveBlocked;
   bool m_isGameOver;
   int m_score;
   SnakeItem* m_snakeFood = nullptr;

private:
   void setGameStatus();
   void startNewGame();
   void gameOver();
   void createFood();


private slots:
   void onMoveSnakeSlot();

};


