#include "mysquare.h"
#include <iostream>


/**
 * Constructor for custom item.
 * @brief MySquare::MySquare
 */
MySquare::MySquare(std::string imagePath, std::string name)
{
    pressed = false;
    QString QImagePath =  QString::fromStdString(imagePath); //converts image path to QString
    image.load(QImagePath); //loads QString image path to this square's pixmap
    this->name = name;
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
    painter->drawPixmap(0,0,125,100,image);
    if(pressed)
    {
        setFlag(GraphicsItemFlag::ItemIsMovable,true);
    }
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
    float x = this->x();
    float height = this->y();
    emit detectCollision(this->name);
    emit sendNewHeightSquare(x, height , name);
}
