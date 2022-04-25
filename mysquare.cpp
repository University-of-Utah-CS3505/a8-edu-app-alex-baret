#include "mysquare.h"
#include <iostream>


/**
 * Constructor for custom item.
 * @brief MySquare::MySquare
 */
MySquare::MySquare(std::string imagePath, std::string name , int initialX, int initialY, int width, int height)
{
    pressed = false;
    QString QImagePath =  QString::fromStdString(imagePath); //converts image path to QString
    image.load(QImagePath); //loads QString image path to this square's pixmap

    this->name = name;

    this->initialXLoc = initialX;
    this->initialYLoc = initialY;

    this->width = width;
    this->height = height;


}

/**
 * Returns the boundaries of the custom shape.
 * @brief MySquare::boundingRect
 * @return
 */
QRectF MySquare::boundingRect() const
{
    return QRectF(this->initialXLoc,this->initialYLoc,this->width,this->height); //outer-most edge of the shape
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
    painter->drawPixmap(initialXLoc,initialYLoc,width,height,image);
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
    emit notifySceneToResize();
    std::cout << "in mouse press" <<std::endl;
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
    emit notifySceneToResize();
    pressed = false;
    update(); //forces object to repaint
    QGraphicsItem::mouseReleaseEvent(event);

    //std::cout << "event info: " << event->pos().x() << " , "<<event->pos().y() << std::endl;

    float x = this->x();
    float y = this->y();

    std::cout << "this object's xloc: " << this->pos().x() << "this object's yloc: "<< this->pos().y() << std::endl;

    //hardcoding in boundaries of scene size. NOTE: if you adjust the scene size you must update these values
    if(x < 560 && x > -620 && y < 340 && y > -390){
        emit detectCollision(this->name);
        emit sendNewHeightSquare(x, y , name);
    }else{
        //set position to initial position
        std::cout << "out of bounds" << std::endl;
        this->setPos(initialXLoc, initialYLoc);
    }


}
