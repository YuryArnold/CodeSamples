#pragma once

#include <QGraphicsView>
#include <QWheelEvent>
#include <QScrollBar>
#include "graphicsscene.h"


class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(QWidget* parent = nullptr);
    void addItem(QGraphicsItem *item);
    void removeItem(QGraphicsItem *item);
    void clear();
protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


private:
    void InitScene();
    bool m_pressed;
    QPoint m_lastMousePos;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
};


