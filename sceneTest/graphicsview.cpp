#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget *parent):QGraphicsView(parent)
{
    this->InitScene();
    this->setCacheMode(CacheNone);
    this->setViewportUpdateMode(BoundingRectViewportUpdate);
    this->setInteractive(true);
    m_pressed = false;
}

void GraphicsView::addItem(QGraphicsItem* item)
{
    this->scene()->addItem(item);
}

void GraphicsView::removeItem(QGraphicsItem* item)
{
    this->scene()->removeItem(item);
}

void GraphicsView::Clear()
{
    this->scene()->clear();
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
    if(Qt::LeftButton == event->button()){
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
    if(Qt::LeftButton == event->button())
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
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    //QWidget* parent = reinterpret_cast<QWidget*>(this->parent());
    //double scaleScene =  static_cast<double>(parent->height())/static_cast<double>(parent->width());
    scene->setSceneRect(-50,-50,100,100);
    this->setScene(scene);
    this->setCacheMode(CacheNone);
    this->setViewportUpdateMode(BoundingRectViewportUpdate);
    this->setRenderHint(QPainter::Antialiasing);
    this->setTransformationAnchor(AnchorUnderMouse);
    this->setMinimumSize(150,150);
    this->setInteractive(true);
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

