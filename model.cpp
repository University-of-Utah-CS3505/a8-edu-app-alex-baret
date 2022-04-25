#include "model.h"
#include "level.h"
#include <iostream>
#include <QPixmap>
#include <string>
#include <vector>


using namespace std;

/**
 * Constructor for model.
 * @brief Model::Model
 * @param parent
 */
Model::Model(QObject *parent)
    : QObject{parent}
{

//=== loading levels ===//
//vector<string> L1Lessons = {"Education about antibiotic medication", "Teaches other cleaning methods"};
//vector<string> L1Treatments = {"hyrdogenPerxoide", "neosporin", "bandAid"};
//vector<string> L1Symptoms = {"No symptoms"};

//QPixmap scraped2;
//QPixmap scraped1;
//QPixmap scraped0;

//vector<QPixmap> L1Stages = {scraped2, scraped1, scraped0};
//lvl1 = new Level("Scratched Knees", L1Lessons, L1Treatments, L1Symptoms, L1Stages, true);

GameReader* levels = new GameReader(":/text/JsonExample_1.txt");
levelList = levels->getLevels();
lvl1 = &levelList[0].second;


//create a 'QGraphicsObject' object for a "patient"
newPatient = new Patient(":/medicines/patient-basic", "patient");


}

/**
 * Slot that detects collisons between treatments and other graphics objects. Accesses treatment by name from the map and detects collision with "other" object.  "Other" will be the "patient".
 * @brief Model::collisionDetectionFromCaller
 * @param nameOfCaller
 */
void Model::collisionDetectionFromCaller(std::string nameOfCaller)
{
    bool collision = treatments.at(nameOfCaller)->collidesWithItem(newPatient);
    if (collision && nameOfCaller != "patient"){
        std::cout << nameOfCaller << " collided with patient" << std::endl;
    }

}

void Model::loadLevel(Level* level){
    //loop through the valid treatments at set the flag in the associating MySquare object as 'won't fall'
    for(auto treatment : lvl1->validTreatments){

        //treatments.at(treatment)->canDrop = false;
    }

    //do other stuff
}

void Model::setTreatmentCanDrop(std::string name, bool canDrop){
    treatments.at(name)->canDrop = canDrop;
}
