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
    void sendNewHeightSquare(float x, float height , std::string name);
    void detectCollision(std::string name);
public:
    MySquare(std::string imagePath, std::string name);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPixmap image;
    bool pressed;
    std::string name;
    bool canMove = true;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MYSQUARE_H
