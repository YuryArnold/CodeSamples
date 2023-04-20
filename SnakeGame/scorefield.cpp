#include "scorefield.h"

ScoreField::ScoreField()
{
    setFixedSize(300,40);
    m_text = "score : 0 \npause = White";
}

void ScoreField::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter;
    painter.begin(this);
    painter.setPen(QColor(250,250,250));
    painter.setFont(QFont("Arial",10,70));
    painter.drawText(QRect(0,0,width(),height()),Qt::AlignCenter,m_text);
    painter.end();
}

void ScoreField::onChangeTextSlot(QString _text)
{
    m_text = _text;
    repaint();
}
