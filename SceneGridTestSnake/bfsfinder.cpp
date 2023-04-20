#include "bfsfinder.h"
#include <algorithm>

BfsFinder::BfsFinder(QObject* parent) : QObject(parent)
{

}

BfsFinder::~BfsFinder()
{

}

void BfsFinder::setStateArray(stateArr& _states)
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
bool BfsFinder::BFSCalculate(Point _src, Point _dest, PointsContainer& _path)
{
    using namespace std;
    int sectorsOfMask = 4;

    QSet<Point> visitedSet;
    queue<shared_ptr<bfsNode>> q;
    if (!states[_src.first][_src.second] || !states[_dest.first][_dest.second])
        return false;
    visitedSet.insert(qMakePair(_src.first,_src.second));

    auto s = make_shared<bfsNode>(_src);

    q.push(s);

    while (!q.empty()) {
        auto curr = q.front();
        Point pt = curr->pt;
        if (pt.first == _dest.first && pt.second == _dest.second){
            _path.append(std::move(curr->pathPoint));
            return true;
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
    return false;
}
