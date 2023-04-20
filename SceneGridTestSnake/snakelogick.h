#ifndef SNAKELOGICK_H
#define SNAKELOGICK_H

#include <QObject>
#include <QVector>
#include <QPair>
#include <QTimer>
#include "bfsfinder.h"
#include "typedefs.h"

class SnakeLogick : public QObject
{
    Q_OBJECT

public:
    explicit SnakeLogick(QObject* parent = nullptr);
    ~SnakeLogick();
    void setTimeOfMovement(int _msec);
    void setGridMatrix(stateArr* _arr);
    void moveSnakeOnScene(bool _flag);
    void generateSnake();
    void reset();

public slots:
    void recalculatePosition();

signals :
    void recalculatedSnake(PointsContainer _snake);

private:
    bool isValid(int row, int col){
        return (row >= 0) && (row < rows) &&
               (col >= 0) && (col < colums);
    }

    bool isSnakeNode(const Point& node){
        auto result = std::find_if(snake.begin(),snake.end(),
                                   [&node](Point& snakeNode){
            return (node.first == snakeNode.first
                && node.second == snakeNode.second);
        });

        return (result != snake.end());
    }

    bool getNearestCells(Point& head,PointsContainer& neighbours);

    PointsContainer snake;
    QTimer updatePosition;
    stateArr* arr;
    BfsFinder findSnakePath;
    int rows;
    int colums;
    int rowNum[4] = {-1, 0, 0, 1};
    int colNum[4] = {0, -1, 1, 0};
};

#endif // SNAKELOGICK_H
