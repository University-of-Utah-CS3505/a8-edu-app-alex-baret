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

      createTreatment("ibuprofen" , ":/medicines/ibuprofen.png");
      createTreatment("hyrdogenPerxoide",":/medicines/hydrogen-peroxide.png" );
      createTreatment("bandAid" , ":/medicines/band-aid.png");
      createTreatment("neosporin", ":/medicines/neosporin.png");

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

    // ======== b2Body Set up ======== //

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 5.0f); // set initial positions

    body = world.CreateBody(&bodyDef);

    body->SetSleepingAllowed(true);

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

    //set its inital velocity to 0
    body->SetLinearVelocity(b2Vec2(0,0));

    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);

    //add the body two the mainModel and map it to 'empty' since nothing has been clicked yet
    mainModel->currentTreatment.first = body;
    mainModel->currentTreatment.second = "empty";

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
       connect(treatment.second,
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

    connect(ui->toggleCanDrop,
            &QPushButton::clicked,
            this,
            &MainWindow::on_toggleCanDrop_clicked);

    mainModel->loadLevel(mainModel->lvl1);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
* Creates a new treatment by creating a MySquare object that holds the image that represents this treatment.
*
* @brief MainWindow::createTreatment
* @param name - the name of the treatment object to be created
* @param imageLoc - the location of where the image you want to be show for the object is
* @param xLoc - the x coordinate of the inital position of the new treatment object
* @param yLoc - the y coordinate of the inital position of the new treatment object
*/
void MainWindow::createTreatment(std::string name , std::string imageLoc)
{
   //create a 'MySquare' object to represent a QGraphics object that can hold an image
   MySquare *newTreatment = new MySquare(imageLoc, name);

   //create a pair that maps the name of the treatment to the graphic object that represents it
   std::pair <std::string,MySquare *> mySquarePair (name,newTreatment);

   //add the pair to the map in the model
   mainModel->treatments.insert(mySquarePair);

   //add the new graphics object to the scene
   scene->addItem(newTreatment);
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

   //when it reaches the ground it has a linear velocity of zero; do not try to update position if there isn't a selected treatment yet
   if(body->GetLinearVelocity().y < 0 && mainModel->currentTreatment.second != "empty"){

       for(auto it = mainModel->lvl1->validTreatments.begin(); it != mainModel->lvl1->validTreatments.end(); ++it){
           if(mainModel->treatments.at(mainModel->currentTreatment.second)->canDrop){

               b2Vec2 position = body->GetPosition();
               emit sendNewHeightValue(position.y*75); //emit the new y value of the body TO THE SLIDER
               //updating the QGraphicsItem to have the bodies' properties
               mainModel->treatments.at(mainModel->currentTreatment.second)->setPos(position.x*75, -position.y*75);

           }
       }
   }
   else{
       body->SetLinearVelocity(b2Vec2(0,0));
   }
}


/**
 * Slot for receiving a new height value.  Resets body heights to height.
 * @brief MainWindow::receiveNewHeightValue
 * @param height
 */
void MainWindow::receiveNewHeightValue(float x, float height, std::string name)
{
    //set the box2D body to assoiciate with the 'treatment' that was clicked on
    mainModel->currentTreatment.second = name;

    b2Vec2 newPos(x/75, -height/75);
    // the real gravity set in startup is never used so im doing this for now
    b2Vec2 fakeGravity(0.0f, -5.0f);

    body->SetTransform(newPos, body->GetAngle());
    body->SetLinearVelocity(fakeGravity);
}



void MainWindow::on_toggleCanDrop_clicked()
{
//   if(ui->marioButton->isChecked()){
//       mainModel->setTreatmentCanDrop("mario" , true);
//   }else{
//       mainModel->setTreatmentCanDrop("mario" , false);
//   }
//   if(ui->luigiButton->isChecked()){
//       mainModel->setTreatmentCanDrop("luigi" , true);
//   }else{
//       mainModel->setTreatmentCanDrop("luigi" , false);
//   }
//   if(ui->peachButton->isChecked()){
//       mainModel->setTreatmentCanDrop("peach" , true);
//   }else{
//       mainModel->setTreatmentCanDrop("peach" , false);
//   }
//   if(ui->toadButton->isChecked()){
//       mainModel->setTreatmentCanDrop("toad", true);
//   }else{
//       mainModel->setTreatmentCanDrop("toad", false);
//   }
}

