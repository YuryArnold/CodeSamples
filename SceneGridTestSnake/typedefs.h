#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <QVector>
#include <QPair>
#include <QMetaType>

using Point = QPair<int,int>;
using stateArr = QVector<QVector<bool>>;
using PointsContainer = QVector<Point>;

Q_DECLARE_METATYPE(PointsContainer)
Q_DECLARE_METATYPE(Point)

#endif // TYPEDEFS_H
