#ifndef MYSQUARE_H
#define MYSQUARE_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QtWidgets>
#include <QtGui>
#include <QGraphicsPixmapItem>
#include <QPixmap>


class MySquare : public QGraphicsObject
{
    Q_OBJECT

signals:
    void sendNewHeightSquare(float x, float height);
public:
    MySquare();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool pressed;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MYSQUARE_H
