#include "snakeitem.h"
#include "variables.h"

SnakeItem::SnakeItem(QObject *parent) : QObject(parent)
{

}

SnakeItem::~SnakeItem()
{

}

void SnakeItem::printSnakeSlot(PointsContainer points)
{
    if (!duringPoints.isEmpty()){
        for(auto& pt : duringPoints)
            items[pt.first * (numbOfElem)  + pt.second]->setState(State::Empty);
    }

    duringPoints = points;
    //Make start and end points during calculation
    auto tail = points.takeFirst();
    auto head = points.takeLast();

    items[head.first * (numbOfElem)  + head.second]->setState(State::Start);
    items[tail.first * (numbOfElem)  + tail.second]->setState(State::End);

    for(auto& pt : points){
        items[pt.first * (numbOfElem)  + pt.second]->setState(State::Path);
    }
}
