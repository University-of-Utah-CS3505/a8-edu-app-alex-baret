#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>


/**
 * Constructor for the "view" object in Model-View architecture.  Contains setup for all graphics items, graphics scene, and a basic Box2D body (to be updated, will not consist of only one dynamicBody).
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) , world(b2Vec2(0.0f, -10.f))
{
    //setting the model
    mainModel = &model;

    // ======== View setup (Qt) ======== //
    ui->setupUi(this);

    scene = new QGraphicsScene(this); //creating a new scene
    ui->graphicsView->setScene(scene); //setting the scene of the graphics view

    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QPen blackPen(Qt::black);
    blackPen.setWidth(6);

    rectangle = scene->addRect(-100,-100,50,50, blackPen, blueBrush);
    rectangle->setFlag(QGraphicsItem::ItemIsMovable);

    // ======== Create all inital 'treatments' ======== //

      createTreatment("mario" , ":/images/mario.png", 0.0f, 5.0f);
      createTreatment("luigi",":/images/luigi" , 0.0f, 5.0f);
      createTreatment("toad" , ":/images/toad" , 0.0f, 5.0f);
      createTreatment("peach", ":/images/peach" , 0.0f, 5.0f);


    // ======== Box2D initial settings ======== //

    // Define the gravity vector.
    b2Vec2 gravity(0.0f, -10.0f);

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -13.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50.0f, 10.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);

    // ======== Connections between signals and slots ======== //

    //connect call from signal to slot
    connect(this, //connects ??
                &MainWindow::sendNewHeightValue,
                ui->verticalSlider,
                &QSlider::setValue);

    //update world based off timer
    timer = new QTimer(this);
       connect(timer,
               &QTimer::timeout,
               this,
               &MainWindow::updateWorld);
       timer->start(10);


    //loop through the 'treatments' from main model, i.e mySquare objects,
    //and connect the singal with slot in mainwindow to move update the object's positions when they're moved
    for ( auto treatment : mainModel->treatments) {
       connect(treatment.second, //connects ??
               &MySquare::sendNewHeightSquare,
               this,
               &MainWindow::receiveNewHeightValue);
       }


    //adding collision detection for each treatment
    for ( auto treatment : mainModel->treatments) {
       connect(treatment.second, //connects initial collision detection signal from the caller to the slot in model
                   &MySquare::detectCollision,
                   mainModel,
                   &Model::collisionDetectionFromCaller);
   }

    mainModel->loadLevel(mainModel->lvl1);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
* Creates a new treatment by first creating a MySquare object that holds the image that represents this treatment. Then,
* this function creates a box 2d body for the MySquare. Finally, the function maps the given name to its corresponding
* MySquare and b2Body objects in the main model.
*
* @brief MainWindow::createTreatment
* @param name - the name of the treatment object to be created
* @param imageLoc - the location of where the image you want to be show for the object is
* @param xLoc - the x coordinate of the inital position of the new treatment object
* @param yLoc - the y coordinate of the inital position of the new treatment object
*/
void MainWindow::createTreatment(std::string name , std::string imageLoc, float xLoc, float yLoc)
{
   //create a 'MySquare' object to represent a QGraphics object that can hold an image
   MySquare *newTreatment = new MySquare(imageLoc, name);

   //create a pair that maps the name of the treatment to the graphic object that represents it
   std::pair <std::string,MySquare *> mySquarePair (name,newTreatment);

   //add the pair to the map in the model
   mainModel->treatments.insert(mySquarePair);

   //add the new graphics object to the scene
   scene->addItem(newTreatment);

   //set up a box 2d body for the new treatment
   b2Body* newBody;

   // Define the dynamic body. We set its position and call the body factory.
   b2BodyDef bodyDef;
   bodyDef.type = b2_dynamicBody;
   bodyDef.position.Set(xLoc, yLoc); // set initial positions

   newBody = world.CreateBody(&bodyDef);

   newBody->SetSleepingAllowed(true);

   // Define another box shape for our dynamic body.
   b2PolygonShape dynamicBox;
   dynamicBox.SetAsBox(1.0f, 1.0f);

   // Define the dynamic body fixture.
   b2FixtureDef fixtureDef;
   fixtureDef.shape = &dynamicBox;

   // Set the box density to be non-zero, so it will be dynamic.
   fixtureDef.density = 1.0f;

   // Override the default friction.
   fixtureDef.friction = 0.3f;

   //set restitution (bounciness)
   fixtureDef.restitution = 0.9f;

   // Add the shape to the body.
   newBody->CreateFixture(&fixtureDef);

   //add the pair: name, b2Body*, to the map of treatment bodies in main model
   std::pair <std::string,b2Body *> box2DPair (name, newBody);
   mainModel->treatmentBodies.insert(box2DPair);
}


/**
 * Slot for update world.
 * @brief updateWorld
 * @param height
 */
void MainWindow::updateWorld(){

   // Instruct the world to perform a single step of simulation.
   // It is generally best to keep the time step and iterations fixed.
   world.Step(timeStep, velocityIterations, positionIterations);

   //iterator through the map of treatmentBodies to check to see if they have been moved
   //NOTE: it->first is the string 'name' that maps to it->second which is the actual b2Body*
   for(auto it = mainModel->treatmentBodies.begin(); it !=mainModel->treatmentBodies.end(); ++it){

       b2Vec2 position = it->second->GetPosition();
       float32 angle = it->second->GetAngle();

       if(it->second->GetLinearVelocity().y < 0 &&  mainModel->treatments.at(it->first)->canMove){ //when it reaches the ground it has a linear velocity of zero
           emit sendNewHeightValue(position.y*75); //emit the new y value of the body TO THE SLIDER

           mainModel->treatments.at(it->first)->setPos(position.x*75, -position.y*75); //updating the QGraphicsItem to have the bodies' properties

           printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);

       }
       else{
           //std::cout << "stopped" << std::endl;
           it->second->SetLinearVelocity(b2Vec2(0,0));
           //timer->stop();
       }
   }
}


/**
 * Slot for receiving a new height value.  Resets body heights to height.
 * @brief MainWindow::receiveNewHeightValue
 * @param height
 */
void MainWindow::receiveNewHeightValue(float x, float height, std::string name)
{
    std::cout << height << std::endl;
    b2Vec2 newPos(x/75, -height/75);
    b2Vec2 fakeGravity(0.0f, -5.0f);          // the real gravity set in startup is never used so im doing this for now
    mainModel->treatmentBodies.at(name)->SetTransform(newPos, mainModel->treatmentBodies.at(name)->GetAngle());
    mainModel->treatmentBodies.at(name)->SetLinearVelocity(fakeGravity);
}



