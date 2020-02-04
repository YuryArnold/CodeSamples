#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <QVector>
#include <QPair>
#include <QMetaType>

using rawPointsContainer = QVector<QPair<double,double>>;
Q_DECLARE_METATYPE(rawPointsContainer)

#endif // TYPEDEFS_H
