#include "model.h"

/**
 * Constructor for model.
 * @brief Model::Model
 * @param parent
 */
Model::Model(QObject *parent)
    : QObject{parent}
{

//=== loading treatments ===//

//creating mario as a pair
std::pair <std::string,MySquare *> mario ("mario",new MySquare(":/images/mario.png"));

//creating luigi as a pair
std::pair <std::string,MySquare *> luigi ("luigi",new MySquare(":/images/luigi"));

treatments.insert(mario); //adding mario
treatments.insert(luigi); //adding luigi

}
