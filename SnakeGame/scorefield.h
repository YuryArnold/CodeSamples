#pragma once
#include <QWidget>
#include <QPainter>

class ScoreField : public QWidget
{
    Q_OBJECT
public:
    ScoreField();

protected:
    void paintEvent(QPaintEvent* event);

public slots:
    void onChangeTextSlot(QString _text);

private:
    QString m_text;
};
