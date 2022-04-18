#include "model.h"
#include <iostream>

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
std::pair <std::string,MySquare *> mario ("mario",new MySquare(":/images/mario.png", "mario"));

//creating luigi as a pair
std::pair <std::string,MySquare *> luigi ("luigi",new MySquare(":/images/luigi", "luigi"));

//creating toad as a pair
std::pair <std::string,MySquare *> toad ("toad",new MySquare(":/images/toad", "toad"));

//creating peach as a pair
std::pair <std::string,MySquare *> peach ("peach",new MySquare(":/images/peach", "peach"));


treatments.insert(mario); //adding mario
treatments.insert(luigi); //adding luigi
treatments.insert(toad); //adding toad
treatments.insert(peach); //adding peach

}

/**
 * Slot that detects collisons between treatments and other graphics objects. Accesses treatment by name from the map and detects collision with "other" object.  "Other" will be the "patient".
 * @brief Model::collisionDetectionFromCaller
 * @param nameOfCaller
 */
void Model::collisionDetectionFromCaller(std::string nameOfCaller)
{
    //std::cout << "in collision detection" << std::endl;
    bool collision = treatments.at(nameOfCaller)->collidesWithItem(treatments.at("luigi"));
    if (collision){
        std::cout << nameOfCaller << " collided with luigi" << std::endl;
    }

}
