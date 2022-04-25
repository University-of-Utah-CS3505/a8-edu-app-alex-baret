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
currentLevel = &levelList[0].second;


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

void Model::loadLevel(){
    //loop through the valid treatments at set the flag in the associating MySquare object as 'won't fall'
    for(auto treatment : currentLevel->validTreatments){

        treatments.at(treatment)->canDrop = false;
    }

    //set pop text for scenario explanation
    //show scenario pop up

    //set text for explanation pop up

    //

    //do other stuff
}

void Model::setTreatmentCanDrop(std::string name, bool canDrop){
    treatments.at(name)->canDrop = canDrop;
}

void Model::showHint()
{
    //loop through all the treatments and check to see if they are invalid or not.
    for(auto it : treatments){
        std::string curr = it.first;

        //if you reach the end of valid treatments then 'curr' is an invalid treatment
        if(std::find(currentLevel->validTreatments.begin(), currentLevel->validTreatments.end() , curr) == currentLevel->validTreatments.end()){
            //randomly decided if this invalid treatment should fall off the shelf

            if(rand() % 2 == 0 && !treatments.at(curr)->hasDropped){
                //set the treatment can drop flag to true and add it to the hints list
                treatments.at(curr)->canDrop = true;
                hints.push_back(curr);
            }
        }
    }
}

void Model::loadNextLevel(){
    //start by resettig all of the treatmets to their initial positions and starting flags
    for(auto it : treatments){
        it.second->setPos(it.second->initialXLoc, it.second->initialYLoc);
        it.second->canDrop = false;
        it.second->hasDropped = false;
    }

    //next, increment the level counter
    levelCount++;

    std::cout<< levelList.size() << std::endl;

    //if user is on the last level, circle back to the first one
    if(levelCount >= levelList.size()){
        std:: cout << "level count > levelList size" << std::endl;
        levelCount = 0;
    }

    //finally, load the next level data into the current level field
    currentLevel = &levelList[levelCount].second;
    loadLevel();
}


