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


GameReader* levels = new GameReader(":/text/LevelDataJson.txt");
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
void Model::collisionDetectionFromCaller(MySquare* caller)
{
    string nameOfCaller = caller->name;
    bool collision = treatments.at(nameOfCaller)->collidesWithItem(newPatient);
    if (collision && nameOfCaller != "patient"){
        std::cout << nameOfCaller << " collided with patient" << std::endl;

        // if name of caller matches one of the valid treatment names, treatment is valid and patient sprite should be changed
        for (string t : currentLevel->validTreatments){
            if (t == nameOfCaller){
                if (newPatient->stage < (int) currentLevel->patientStagesImages.size()-1){
                    std::cout << "   VALID TREATMENT" << std::endl;
                    newPatient->stage++;
                    newPatient->image.load(QString::fromStdString(currentLevel->patientStagesImages[newPatient->stage].second));
                    newPatient->update();
                    caller->setPos(5000,-5000);
                    return;
                }
                else if (newPatient->stage >= (int) currentLevel->patientStagesImages.size()-1){
                    std::cout << "   LEVEL COMPLETED" << std::endl;
                    newPatient->image.load(QString::fromStdString(":/medicines/patient-basic"));
                    newPatient->update();
                    QTimer::singleShot(2000, this, &Model::loadNextLevel);
                    caller->setPos(5000,-5000);
                    return;
                }
            }
        }
        // else treatment is invalid
        if (wrongGuesses < 1){
            std::cout << "   WRONG TREATMENT" << std::endl;
        }
        else {
            std::cout << "   LEVEL FAILED" << std::endl;
            QTimer::singleShot(2000, this, &Model::loadNextLevel);
        }
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

    //set patient image
    newPatient->stage = 0;
    newPatient->image.load(QString::fromStdString(currentLevel->patientStagesImages[0].second));
    newPatient->update();

    //reset wrong guess counter
    wrongGuesses = 0;

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
        it.second->canDrop = true;
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

/**
 * Handles incorrect answer from mySquare signal.
 * @brief Model::handleIncorrectAnswer
 */
void Model::handleIncorrectAnswer()
{
    answeredIncorrectly = true;
    wrongGuesses++;
    showHint();
}


