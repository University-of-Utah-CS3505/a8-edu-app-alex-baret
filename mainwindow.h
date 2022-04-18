#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Box2D/Box2D.h>
#include <stdio.h>
#include <QTimer>
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include "mysquare.h"
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void sendNewHeightValue(int height);
public slots:
    void updateWorld();
    void receiveNewHeightValue(float x, float height);

public:
    b2World world;
    MainWindow(Model& model, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Model *mainModel;
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;

    //treatments (characters for now)
    std::vector<MySquare *> treatments;
    MySquare *square;
    MySquare *otherSquare;
    MySquare *toadSquare;
    MySquare *peachSquare;


    QGraphicsPixmapItem *pic;
    QTimer *timer;
    b2Body* body;

    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;


};
#endif // MAINWINDOW_H
