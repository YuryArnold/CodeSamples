#include "snakegame.h"

SnakeField::SnakeField()
{
    setFixedSize(300,300);
    setFocusPolicy(Qt::StrongFocus);
    m_snakeItemSize = 10;
    m_fieldSize = width()/m_snakeItemSize;
    startNewGame();
}

void SnakeField::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QBrush gameFieldBrush(QColor(139,144,163),Qt::SolidPattern);
    QBrush snakeBrush(QColor(69,164,147),Qt::SolidPattern);
    QBrush foodBrush(QColor(247,103,123),Qt::SolidPattern);


    QPainter painter;
    painter.begin(this);
    if(m_isGameOver) {
        painter.setPen(QColor(250,250,250));
        painter.setFont(QFont("Arial",15,70));
        painter.drawText(QRect(0,0,
                               width(),
                               height()),
                               Qt::AlignCenter,
                               "GAME OVER\nscore : " + QString::number(m_score));
    }

    painter.setBrush(gameFieldBrush);
    painter.setPen(QColor(50,50,50));
    painter.drawRect(0,0,width() - 1,height() - 1);

    painter.setBrush(snakeBrush);
    painter.setPen(QColor(50,50,50));
    for(int ind = 0; ind < m_snakePtr->m_snakeBody.size(); ++ind)
        painter.drawEllipse(m_snakePtr->m_snakeBody[ind]->m_X * m_snakeItemSize,
                            m_snakePtr->m_snakeBody[ind]->m_Y * m_snakeItemSize,
                            10,10);


    painter.setBrush(foodBrush);
    painter.drawEllipse(m_snakeFood->m_X * m_snakeItemSize,
                        m_snakeFood->m_Y * m_snakeItemSize,
                        m_snakeItemSize,
                        m_snakeItemSize);

    painter.end();
    m_isMoveBlocked = false;
}

void SnakeField::keyPressEvent(QKeyEvent *event)
{
    //pause
    if(event->key() == Qt::Key_Space) {

        if(m_isGameOver) {
            startNewGame();
            return;
        }

        m_isPause = !m_isPause;
        setGameStatus();
    }

    if(m_isMoveBlocked)
        return;

    if(event->key() == Qt::Key_Left && m_snakePtr->m_snakeDirection != Snake::SnakeDirection::right)
        m_snakePtr->m_snakeDirection = Snake::SnakeDirection::left;
    else if(event->key() == Qt::Key_Right && m_snakePtr->m_snakeDirection != Snake::SnakeDirection::left)
        m_snakePtr->m_snakeDirection = Snake::SnakeDirection::right;
    else if(event->key() == Qt::Key_Down && m_snakePtr->m_snakeDirection != Snake::SnakeDirection::up)
        m_snakePtr->m_snakeDirection = Snake::SnakeDirection::down;
    else if(event->key() == Qt::Key_Up && m_snakePtr->m_snakeDirection != Snake::SnakeDirection::down)
        m_snakePtr->m_snakeDirection = Snake::SnakeDirection::up;

    m_isMoveBlocked = true;
}

void SnakeField::setGameStatus()
{
    QString text;
    if(m_isPause) {
        m_moveSnakeTimerPtr->stop();
        text ="score : " + QString::number(m_score) + "\ncontinue - SPACE";
    } else {
        m_moveSnakeTimerPtr->start(100);
        text ="score : " + QString::number(m_score) + "\npause - SPACE";
    }

    emit changeTextSignal(text);

}

void SnakeField::startNewGame()
{
    m_score = 0;
    m_isPause = false;
    m_isMoveBlocked = false;
    m_isGameOver = false;
    m_snakePtr = new Snake();
    m_snakeFood = new SnakeItem(m_fieldSize/2,m_fieldSize/2);
    m_moveSnakeTimerPtr = new QTimer(this);
    connect(m_moveSnakeTimerPtr,
            &QTimer::timeout,
            this,
            &SnakeField::onMoveSnakeSlot);
    m_moveSnakeTimerPtr->start(100);

    QString text = "Score : " + QString::number(m_score) + "\npause - SPACE";
    emit changeTextSignal(text);
}

void SnakeField::gameOver()
{
    m_isGameOver = true;
    QString text = "start again - SPACE";
    emit changeTextSignal(text);
    m_moveSnakeTimerPtr->stop();
    delete m_snakeFood;
    delete m_snakePtr;
    delete m_moveSnakeTimerPtr;
}

void SnakeField::createFood()
{
    m_snakeFood->m_X = QRandomGenerator::global()->bounded(0,m_fieldSize - 1);
    m_snakeFood->m_Y = QRandomGenerator::global()->bounded(0,m_fieldSize - 1);

    for (int ind = 0; ind < m_snakePtr->m_snakeBody.size(); ++ind)
        if(m_snakeFood->m_X == m_snakePtr->m_snakeBody[ind]->m_X
          && m_snakeFood->m_Y == m_snakePtr->m_snakeBody[ind]->m_Y)
            return createFood();
}

void SnakeField::onMoveSnakeSlot()
{
    SnakeItem* newSnakeItem = nullptr;
    if(m_snakePtr->m_snakeDirection == Snake::SnakeDirection::right)
        newSnakeItem = new SnakeItem(m_snakePtr->m_snakeBody[0]->m_X + 1,
                                     m_snakePtr->m_snakeBody[0]->m_Y);
    else if(m_snakePtr->m_snakeDirection == Snake::SnakeDirection::left)
        newSnakeItem = new SnakeItem(m_snakePtr->m_snakeBody[0]->m_X - 1,
                                     m_snakePtr->m_snakeBody[0]->m_Y);
    else if(m_snakePtr->m_snakeDirection == Snake::SnakeDirection::up)
        newSnakeItem = new SnakeItem(m_snakePtr->m_snakeBody[0]->m_X,
                                     m_snakePtr->m_snakeBody[0]->m_Y - 1);
    else
        newSnakeItem = new SnakeItem(m_snakePtr->m_snakeBody[0]->m_X,
                                      m_snakePtr->m_snakeBody[0]->m_Y + 1);

    if(newSnakeItem->m_X >= m_fieldSize)
        newSnakeItem->m_X = 0;
    else if(newSnakeItem->m_X < 0)
        newSnakeItem->m_X = m_fieldSize - 1;
    else if(newSnakeItem->m_Y >= m_fieldSize)
        newSnakeItem->m_Y = 0;
    else if(newSnakeItem->m_Y < 0)
        newSnakeItem->m_Y = m_fieldSize - 1;

    //lose
    for (int ind = 0; ind < m_snakePtr->m_snakeBody.size(); ++ind)
        if(newSnakeItem->m_X == m_snakePtr->m_snakeBody[ind]->m_X
          && newSnakeItem->m_Y == m_snakePtr->m_snakeBody[ind]->m_Y){
            gameOver();
            return;
        }

    if(newSnakeItem->m_X == m_snakeFood->m_X && newSnakeItem->m_Y == m_snakeFood->m_Y) {
        m_score++;
        createFood();
        QString text = "score : " + QString::number(m_score) + "\npause - SPACE";
        emit changeTextSignal(text);
    } else
         m_snakePtr->m_snakeBody.removeLast();


    m_snakePtr->m_snakeBody.insert(0,newSnakeItem);
    repaint();
}

SnakeItem::SnakeItem(int _x, int _y) :
    m_X(_x),
    m_Y(_y)
{

}

Snake::Snake()
{
    m_snakeStartSize = 4;
    for(int ind = 0; ind < m_snakeStartSize; ++ind)
        m_snakeBody.insert(0, new SnakeItem(ind,0));
    m_snakeDirection = right;
}
