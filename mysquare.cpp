#include "mysquare.h"
#include <iostream>


/**
 * Constructor for custom item.
 * @brief MySquare::MySquare
 */
MySquare::MySquare()
{
    pressed = false;
}

/**
 * Returns the boundaries of the custom shape.
 * @brief MySquare::boundingRect
 * @return
 */
QRectF MySquare::boundingRect() const
{
    return QRectF(0,0,100,100); //outer-most edge of the shape
}

/**
 * Paints the custom item.
 * @brief MySquare::paint
 * @param painter
 * @param option
 * @param widget
 */
void MySquare::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush (Qt::blue);

    if(pressed)
    {
        brush.setColor(Qt::red);
        setFlag(GraphicsItemFlag::ItemIsMovable,true);
    }
    else{
        brush.setColor(Qt::green);
    }
    painter->fillRect(rec,brush);
    painter->drawRect(rec);



}

/**
 * Mouse pressed event handler, repaints object and notifies other listeners that the mouse was pressed.
 * @brief MySquare::mousePressEvent
 * @param event
 */
void MySquare::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update(); //forces object to repaint
    QGraphicsItem::mousePressEvent(event);
}

/**
 * Mouse pressed event handler, repaints object and notifies other listeners that the mouse was pressed.
 * @brief MySquare::mouseReleaseEvent
 * @param event
 */
void MySquare::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update(); //forces object to repaint
    QGraphicsItem::mouseReleaseEvent(event);

    std::cout << "event info: " << event->pos().y() << std::endl;
    float x = event->pos().x();
    float height = event->pos().y();
    emit sendNewHeightSquare(x, height);
}
