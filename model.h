#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "mysquare.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    std::map<std::string, MySquare*> treatments; //string is name of the treatment and MySquare pointer is a pointer for the actual custom graphics view object to be added in model constructor

private:


signals:

};

#endif // MODEL_H
