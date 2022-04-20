#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "mysquare.h"
#include <Box2D/Box2D.h>
#include "level.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    std::map<std::string, MySquare*> treatments; //string is name of the treatment and MySquare pointer is a pointer for the actual custom graphics view object to be added in model constructor
    std::map<std::string, b2Body* > treatmentBodies;

    Level* lvl1;

public slots:
    void collisionDetectionFromCaller(std::string nameOfCaller);
    void loadLevel(Level* level);

private:
std::vector<std::string> correctTreatmentOrder;



signals:

};

#endif // MODEL_H
