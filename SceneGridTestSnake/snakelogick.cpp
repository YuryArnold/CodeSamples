#include "snakelogick.h"

SnakeLogick::SnakeLogick(QObject *parent) : QObject(parent)
{
    connect(&updatePosition,&QTimer::timeout,
            this,&SnakeLogick::recalculatePosition);
}

SnakeLogick::~SnakeLogick()
{
    updatePosition.stop();
}

void SnakeLogick::setTimeOfMovement(int _msec)
{
    updatePosition.setInterval(_msec);
}


/*Week place, think about that later! Talk with a Kostya!*/
void SnakeLogick::setGridMatrix(stateArr *_arr)
{
    arr = _arr;
    rows = (*_arr).size();
    colums = (*_arr)[0].size();
    findSnakePath.setStateArray(*arr);
}

void SnakeLogick::moveSnakeOnScene(bool _flag)
{
    (_flag)?(updatePosition.start())
          :(updatePosition.stop());
}

void SnakeLogick::generateSnake()
{
    while (true) {
        Point head = qMakePair(rand()%100,rand()%100);
        Point tail = qMakePair(rand()%100,rand()%100);
        if(findSnakePath.BFSCalculate(head,tail,snake))
            break;
    }
}

void SnakeLogick::reset()
{
    updatePosition.stop();
    snake.clear();
}

/*Also need To talk about this function qith Kostya*/
void SnakeLogick::recalculatePosition()
{
    static int reverseCnt = 0;

    PointsContainer neighbours;

    if(reverseCnt){
        auto first = snake[snake.size()-1];
        getNearestCells(first,neighbours);
        auto elem = neighbours[rand()%neighbours.size()];
        snake.push_back(elem);
        snake.removeFirst();
        reverseCnt--;
    } else {
        auto first = snake[0];
        if(getNearestCells(first,neighbours)){
            auto elem = neighbours[rand()%neighbours.size()];
            snake.push_front(elem);
            snake.removeLast();
        } else {
            reverseCnt = 5;
        }
    }
    emit recalculatedSnake(snake);
}

bool SnakeLogick::getNearestCells(Point& first,PointsContainer& neighbours)
{
    int sectorsOfMask = 4;
    for(int i = 0 ; i < sectorsOfMask; ++i){
        int row = first.first + rowNum[i];
        int col = first.second + colNum[i];
        if(isValid(row,col) &&
                (*arr)[row][col] &&
                !isSnakeNode(qMakePair(row,col))){
            neighbours.push_back(qMakePair(row,col));
        }
    }

    return !neighbours.isEmpty();
}
