#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) , world(b2Vec2(0.0f, -10.f))
{
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

    //custom item (creation and addition to scene)
    square = new MySquare(":/images/mario.png");
    scene->addItem(square);

    //addition of other square
    otherSquare = new MySquare(":/images/luigi");
    scene->addItem(otherSquare);

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

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 5.0f);
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

    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);

    // ======== Connections between signals and slots ======== //

    //connect call from signal to slot
    connect(this, //connects blue push button to update Model's bluePress slot
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

   //connect call from signal to slot
   connect(square, //connects blue push button to update Model's bluePress slot
               &MySquare::sendNewHeightSquare,
               this,
               &MainWindow::receiveNewHeightValue);

}

MainWindow::~MainWindow()
{
    delete ui;
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
    b2Vec2 position = body->GetPosition();
    float32 angle = body->GetAngle();

    if(body->GetLinearVelocity().y < 0){ //when it reaches the ground it has a linear velocity of zero
        emit sendNewHeightValue(position.y*75); //emit the new y value of the body TO THE SLIDER

        square->setPos(position.x*75, -position.y*75); //updating the QGraphicsItem to have the bodies' properties

        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);

    }
    else{
        //std::cout << "stopped" << std::endl;
        body->SetLinearVelocity(b2Vec2(0,0));
        //timer->stop();
    }
}

/**
 * Slot for receiving a new height value.  Resets body heights to height.
 * @brief MainWindow::receiveNewHeightValue
 * @param height
 */
void MainWindow::receiveNewHeightValue(float x, float height)
{
    std::cout << height << std::endl;
    b2Vec2 newPos(x/5, height/10);
    b2Vec2 fakeGravity(0.0f, -5.0f);          // the real gravity set in startup is never used so im doing this for now
    body->SetTransform(newPos, body->GetAngle());
    body->SetLinearVelocity(fakeGravity);
}



