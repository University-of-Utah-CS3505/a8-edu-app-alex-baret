#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "mysquare.h"
#include <Box2D/Box2D.h>
#include "level.h"
#include "gamereader.h"
#include "patient.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    std::map<std::string, MySquare*> treatments; //string is name of the treatment and MySquare pointer is a pointer for the actual custom graphics view object to be added in model constructor
    std::pair<b2Body* , std::string> currentTreatment;
    Level* lvl1;
    Patient *newPatient;
    void setTreatmentCanDrop(std::string name, bool canDrop);

public slots:
    void collisionDetectionFromCaller(std::string nameOfCaller);
    void loadLevel(Level* level);

private:
std::vector<std::string> correctTreatmentOrder;
std::vector<pair<std::string, Level>> levelList;



signals:

};

#endif // MODEL_H
