#include "bfsfinder.h"
#include <algorithm>

BfsFinder::BfsFinder(stateArr _states,
                     QObject* parent) : QObject(parent)
{
    rows = _states.size();
    colums = _states[0].size();
    states = _states;
}

BfsFinder::~BfsFinder()
{

}

void BfsFinder::setStateArray(stateArr _states)
{
    rows = _states.size();
    colums = _states[0].size();
    states = _states;
}

/**
 * @brief BfsFinder::BFSCalculate
 * Used bfs algorithm,
 * but Lee algorithm can be used also.
 * As good architecture desition I could
 * transfer it in to std::function.
 * Just for simplicity.
 * @param src
 * @param dest
 */
void BfsFinder::BFSCalculate(Point src, Point dest)
{
    using namespace std;
    int sectorsOfMask = 4;

    QSet<Point> visitedSet;
    queue<shared_ptr<bfsNode>> q;
    if (!states[src.first][src.second] || !states[dest.first][dest.second])
        emit noPathSignal();
    visitedSet.insert(qMakePair(src.first,src.second));

    auto s = make_shared<bfsNode>(src);

    q.push(s);

    while (!q.empty()) {
        auto curr = q.front();
        Point pt = curr->pt;
        if (pt.first == dest.first && pt.second == dest.second){
            emit setCalculatedPath(curr->pathPoint);
            return;
        }
        q.pop();
        for (int i = 0; i < sectorsOfMask; i++) {
            int row = pt.first + rowNum[i];
            int col = pt.second + colNum[i];
            if (isValid(row, col) && states[row][col] &&
              (visitedSet.find(qMakePair(row,col)) == visitedSet.end())) {
                visitedSet.insert(qMakePair(row,col));
                auto Adjcell = make_shared<bfsNode>(qMakePair(row,col));
                Adjcell->pathPoint.append(curr->pathPoint);
                q.push(Adjcell);
            }
        }
    }
    emit noPathSignal();
}
