/**
 *3505 A8 Education App
 * Team Fresca (Alex, Alivia, Ian, Joey)
 * 5/3/22
 *
 * model.cpp
 *
 * File reader that converts JSON objects from LevelDataJson.txt to Level objects.
 **/

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
 * Slot that detects collisons between treatments and other graphics objects. Accesses treatment by name from the map and detects collision with "other" object. "Other" will be the "patient".
 * @brief Model::collisionDetectionFromCaller
 * @param nameOfCaller
 */
void Model::collisionDetectionFromCaller(MySquare* caller)
{
    string nameOfCaller = caller->name;
    bool collision = treatments.at(nameOfCaller)->collidesWithItem(newPatient);

    // If there is a collision and the collider isn't the patient
    if (collision && nameOfCaller != "patient"){

        // And if the name of caller matches one of the valid treatment names, treatment is valid and patient sprite should be changed.
        // Must also satisfy the requirement of being in the correct order if necessary
        if (currentLevel->inOrder){
            if (neededTreatment[newPatient->stage] == nameOfCaller){
                collisionHelper(caller);
                return;
            }
        }
        else {
            if (std::find(neededTreatment.begin(), neededTreatment.end(), nameOfCaller) != neededTreatment.end()) {
                collisionHelper(caller);
                return;
            }
        }

        // Else treatment is invalid
        if (wrongGuesses < 2){
            std::cout << "   WRONG TREATMENT" << std::endl;
        }
        // Present player with tutorial if level is failed after three attempts
        else {
            std::cout << "   LEVEL FAILED" << std::endl;
            levelPassed->setText("Not quite! Here are the steps to solve.");
            QTimer::singleShot(0000, this, &Model::EmitShowPop);
        }
    }

}

/**
 * Determines if the level was completed or if a step was completed. Responds accordingly.
 * @brief Model::collisionHelper
 * @param caller
 */
void Model::collisionHelper(MySquare* caller){
    // Updates the patient image and hides the used treatment
    // STEP COMPLETED
    if (currentLevel->patientStagesImages[newPatient->stage+1].first != "H"){
        newPatient->stage++;
        newPatient->image.load(QString::fromStdString(currentLevel->patientStagesImages[newPatient->stage].second));
        newPatient->update();
        caller->setPos(5000,-5000);
        return;
    }
    // Shows the player the lesson if the level is finished
    // LEVEL COMPLETED
    else if (currentLevel->patientStagesImages[newPatient->stage+1].first == "H"){
        levelPassed->setText("Good Job! You passed the Level!");
        newPatient->image.load(QString::fromStdString(currentLevel->patientStagesImages[newPatient->stage+1].second));
        newPatient->update();
        QTimer::singleShot(1000, this, &Model::EmitShowPop);
        caller->setPos(5000,-5000);
        return;
    }
}

/**
 * Emits the signal to show the lesson pop-up
 * @brief Model::EmitShowPop
 */
void Model::EmitShowPop(){
    emit showPopSignal();
}

/**
 * Prepares and loads in the current level
 * @brief Model::loadLevel
 */
void Model::loadLevel(){

    emit loadUI();

    // Loop through the valid treatments and set the flag in the associating MySquare object as 'won't fall'
    for(auto treatment : currentLevel->validTreatments){
        treatments.at(treatment)->canDrop = false;
    }

    // Set patient image
    newPatient->stage = 0;
    newPatient->image.load(QString::fromStdString(currentLevel->patientStagesImages[0].second));
    newPatient->update();

    // Set the currently needed treatment
    neededTreatment.clear();
    if (!currentLevel->inOrder){
        neededTreatment = currentLevel->validTreatments;
    }
    // Order the needed treatments if neccesary
    else {
        for (pair<string, string> p : currentLevel->medicineToStage){
            neededTreatment.push_back(p.first);
        }
    }

    // Reset the wrong guess counter
    wrongGuesses = 0;

}

/**
 * Enable or disable gravity for a given treatment
 * @brief Model::setTreatmentCanDrop
 * @param name
 * @param canDrop
 */
void Model::setTreatmentCanDrop(std::string name, bool canDrop){
    treatments.at(name)->canDrop = canDrop;
}

/**
 * Show the player a hint by dropping a random selection of non-valid treatments from the shelf.
 * @brief Model::showHint
 */
void Model::showHint()
{
    // Loop through all the treatments and check to see if they are invalid or not.
    for(auto it : treatments){
        std::string curr = it.first;

        // If you reach the end of valid treatments then 'curr' is an invalid treatment
        if(std::find(currentLevel->validTreatments.begin(), currentLevel->validTreatments.end() , curr) == currentLevel->validTreatments.end()){

            // Randomly decide if this invalid treatment should fall off the shelf.
            if(rand() % 2 == 0 && !treatments.at(curr)->hasDropped){
                // Set the treatment can drop flag to true and add it to the hints list
                treatments.at(curr)->canDrop = true;
                hints.push_back(curr);
            }
        }
    }
}

/**
 * Resets the game and determines which level to load in next.
 * @brief Model::loadNextLevel
 */
void Model::loadNextLevel(){
    // Start by reseting all of the treatmets to their initial positions and starting flags
    for(auto it : treatments){
        it.second->setPos(it.second->initialXLoc, it.second->initialYLoc);
        it.second->canDrop = true;
        it.second->hasDropped = false;
    }

    // Next, increment the level counter
    levelCount++;

    // If user is on the last level, circle back to the first one
    if(levelCount >= levelList.size()){
        std:: cout << "level count > levelList size" << std::endl;
        levelCount = 0;
    }

    // Finally, load the next level data into the current level field
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
