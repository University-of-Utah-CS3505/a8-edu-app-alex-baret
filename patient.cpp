/**
 * 3505 A8 Education App
 * Team Fresca (Alex, Alivia, Ian, Joey)
 * 5/3/22
 *
 * patient.cpp
 *
 * Custom QGraphicsObject will coordinate tracking and mouse clicking.
 * Used as a Box2D body.
 *
 **/

#include "patient.h"
#include <iostream>


/**
 * Constructor for custom item.
 * @brief Patient::Patient
 */
Patient::Patient(std::string imagePath, std::string name)
{
    pressed = false;
    QString QImagePath =  QString::fromStdString(imagePath); //converts image path to QString
    image.load(QImagePath); //loads QString image path to this square's pixmap
    this->name = name;
    this->setPos(-150,-200);
    stage = 0;
}

/**
 * Returns the boundaries of the custom shape.
 * @brief Patient::boundingRect
 * @return
 */
QRectF Patient::boundingRect() const
{
    return QRectF(0,-50,300,550); //outer-most edge of the shape
}

/**
 * Paints the custom item.
 * @brief Patient::paint
 * @param painter
 * @param option
 * @param widget
 */
void Patient::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0,-50,300,550,image);
    if(pressed)
    {
        setFlag(GraphicsItemFlag::ItemIsMovable,true);
    }
}

