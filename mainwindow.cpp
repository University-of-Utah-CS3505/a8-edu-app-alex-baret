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
    setSceneSize();
    ui->graphicsView->setScene(scene); //setting the scene of the graphics view


    //background image setup
    QString QImagePath =  ":/medicines/background";
    image.load(QImagePath);
    QPalette palette;
    palette.setBrush(QPalette::Window, image.scaled(this->width(), this->height()));
    setPalette(palette);

    //adds patient to scene
    scene->addItem(mainModel->newPatient);

    // ======== Create all inital 'treatments' ======== //

    //first shelf (left to right)
    createTreatment("coldPack", ":/medicines/cold-pack.png", 160, -85, 50, 62);
    createTreatment("peptoBismol", ":/medicines/pepto-bismol.png", 190, -85, 50, 62);
    createTreatment("water", ":/medicines/water.png", 220, -85, 50, 62);
    createTreatment("neosporin", ":/medicines/neosporin.png", 250, -85, 50, 62);

    //second shelf (left to right)
      createTreatment("allergy" , ":/medicines/allergy.png" , 160, -20, 50, 62 );
      createTreatment("aloeVera",":/medicines/aloe-vera.png", 190, -20, 50, 62 );
      createTreatment("bandage" , ":/medicines/bandage.png", 220, -20, 50, 62);
      createTreatment("bandAid", ":/medicines/band-aid.png", 250, -20, 50, 62);
    //third shelf (left to right)
      createTreatment("coughDrops", ":/medicines/cough-drops.png", 160, 35, 50, 62);
      createTreatment("hotPack", ":/medicines/hot-pack.png", 190, 35, 50, 62);
      createTreatment("hydrogenPeroxide", ":/medicines/hydrogen-peroxide.png", 220, 35, 50, 62);
     createTreatment("ibuprofen", ":/medicines/ibuprofen.png", 250, 35, 50, 62);
      //fourth shelf (left to right)
     createTreatment("naselSpray", ":/medicines/nasel-spray.png", 160, 100, 50, 62);


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
    groundBox.SetAsBox(50.0f, 8.0f);

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


    //update world based off timer
    timer = new QTimer(this);
       connect(timer,
               &QTimer::timeout,
               this,
               &MainWindow::updateWorld);
       timer->start(10);


       for ( auto treatment : mainModel->treatments) {
          connect(treatment.second,
                  &MySquare::notifySceneToResize,
                  this,
                  &MainWindow::setSceneSize);
          }


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






    // SET UP POPUP

    GameReader* lv = new GameReader(":/text/ProjectLevelData.txt");
    std::vector<pair<std::string, Level>> ll = lv->getLevels();
    Level *lvl = &ll[0].second;
    ui->MainPopup->setStyleSheet("background-color: rgb(249, 233, 216); border-radius: 30px;");
    ui->MainPopup->hide();
    loadLevelUI(lvl);

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
void MainWindow::createTreatment(std::string name , std::string imageLoc, int xLoc, int yLoc, int xDim, int yDim)
{
   //create a 'MySquare' object to represent a QGraphics object that can hold an image
   MySquare *newTreatment = new MySquare(imageLoc, name, xLoc, yLoc, xDim, yDim);

   //set the inital position of the treatment
   newTreatment->setPos(xLoc, yLoc);

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
               //emit sendNewHeightValue(position.y*75); //emit the new y value of the body TO THE SLIDER
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

/**
 * Slot for scene resize events.  Connects to MySquare notifySceneToResize signal.
 * @brief MainWindow::callSceneResize
 */
void MainWindow::callSceneResize()
{
    setSceneSize();
}



void MainWindow::on_toggleCanDrop_clicked()
{
    if(ui->marioButton->isChecked()){
        mainModel->setTreatmentCanDrop("ibuprofen" , true);
    }else{
        mainModel->setTreatmentCanDrop("ibuprofen" , false);
    }
    if(ui->luigiButton->isChecked()){
        mainModel->setTreatmentCanDrop("hydrogenPeroxide" , true);
    }else{
        mainModel->setTreatmentCanDrop("hydrogenPeroxide" , false);
    }
    if(ui->peachButton->isChecked()){
        mainModel->setTreatmentCanDrop("bandAid" , true);
    }else{
        mainModel->setTreatmentCanDrop("bandAid" , false);
    }
    if(ui->toadButton->isChecked()){
        mainModel->setTreatmentCanDrop("neosporin", true);
    }else{
        mainModel->setTreatmentCanDrop("neosporin", false);
    }
}

/**
 * Resets the size of the GraphicView's scene.  Used to repaint the scene so that treatments overlapping the patient don't paint over the patient.
 * @brief MainWindow::resetSceneSize
 */
void MainWindow::setSceneSize()
{
    if (expandedX % 2 == 0){
        expandedX ++;
    }
    else{
        expandedX --;
    }
    QPoint sizeConstraint1 = QPoint(-500,500);
    QPoint sizeConstraint2 = QPoint(expandedX, -500);
    QRectF sceneSize = QRectF(sizeConstraint1, sizeConstraint2);
    scene->setSceneRect(sceneSize);
}




void MainWindow::loadLevelUI(Level *level){
    stepsPopLayout = new QVBoxLayout();
    QLabel *stitle = new QLabel();
    stitle->setText("Steps to Solve a " + QString::fromStdString(level->title) + ":");
    QGridLayout *gl = new QGridLayout();
    for (int i = 0; i < level->stepsImages.size(); i++)
    {
        QLabel *label = new QLabel();
        label->setFixedHeight(100);
        label->setFixedWidth(100);
        QString s = QString::fromStdString(level->stepsImages[i].second);
        QPixmap pixmap(s);
        pixmap = pixmap.scaled(label->size(),Qt::KeepAspectRatio);
        label->setPixmap(pixmap);
        gl->addWidget(label, i, 0);

        QLabel *tLabel = new QLabel();
        tLabel->setStyleSheet("font-size: 20px; color: tan; font-family: 'Fuzzy Bubbles', cursive;");
        tLabel->setWordWrap(true);
        tLabel->setText(QString::fromStdString(level->stepsText[i]));
        gl->addWidget(tLabel, i, 1);
    }

    stepsPopLayout->addWidget(stitle);
    stepsPopLayout->addLayout(gl);
    stepsPopLayout->addWidget(stitle);
    ui->MainPopup->setLayout(stepsPopLayout);
    ui->MainPopup->show();


}
