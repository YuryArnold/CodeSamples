#include "graphicsview.h"
#include "variables.h"

GraphicsView::GraphicsView(QWidget *parent):QGraphicsView(parent)
{
    InitScene();
    setCacheMode(CacheNone);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setInteractive(true);
    m_pressed = false;
}

void GraphicsView::addItem(QGraphicsItem* item)
{
    scene()->addItem(item);
}

void GraphicsView::removeItem(QGraphicsItem* item)
{
    scene()->removeItem(item);
}

void GraphicsView::clear()
{
    qobject_cast<GraphicsScene*>(scene())->resetScene();
    scene()->clear();
}

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect)
    painter->setPen(QPen(Qt::black));
}

/**
 * @brief GraphicsView::wheelEvent
 * @param event
 */
void GraphicsView::wheelEvent(QWheelEvent *event)
{
    QGraphicsView::wheelEvent(event);
    if (event->isAccepted()) {
        return;
    }
    const qreal factor = 1.1;
    if (event->angleDelta().y() >0) {
        scale(factor, factor);
    } else {
        scale(1 / factor, 1 / factor);
    }
    event->accept();

}

/**
 * @brief GraphicsView::mousePressEvent
 * @param event
 */
void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(Qt::MiddleButton == event->button()){
        m_pressed = true;
        m_lastMousePos = event->pos();
    }
    QGraphicsView::mousePressEvent(event);
}

/**
 * @brief GraphicsView::mouseReleaseEvent
 * @param event
 */
void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(Qt::MiddleButton == event->button())
        m_pressed = false;
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_pressed)
        return QGraphicsView::mouseMoveEvent(event);

    QPoint diff = m_lastMousePos - event->pos();
    if(QScrollBar* hbar = horizontalScrollBar())
        hbar->setValue(hbar->value() + diff.x());
    if(QScrollBar* vbar = verticalScrollBar())
        vbar->setValue(vbar->value() + diff.y());
    m_lastMousePos = event->pos();
    return QGraphicsView::mouseMoveEvent(event);
}

/**
 * @brief GraphicsView::InitScene
 */
void GraphicsView::InitScene()
{
    QGraphicsScene* scene = new GraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-sceneWidth/2,-sceneHeight/2,sceneWidth,sceneHeight);
    setScene(scene);
    setCacheMode(CacheNone);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setMinimumSize(150,150);
    setInteractive(true);
    centerOn(0,0);
}

/**
 * @brief GraphicsView::resizeEvent
 * For resize scene with all items in it.
 * @param event
 */
void GraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(event);
}

