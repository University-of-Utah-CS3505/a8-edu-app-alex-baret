#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "mysquare.h"
#include <Box2D/Box2D.h>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    std::map<std::string, MySquare*> treatments; //string is name of the treatment and MySquare pointer is a pointer for the actual custom graphics view object to be added in model constructor
    std::map<std::string, b2Body* > treatmentBodies;
public slots:
    void collisionDetectionFromCaller(std::string nameOfCaller);

private:



signals:

};

#endif // MODEL_H
