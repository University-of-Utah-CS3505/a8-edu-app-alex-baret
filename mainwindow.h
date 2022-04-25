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
#include "patient.h"
#include "model.h"
#include <iostream>

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
    void receiveNewHeightValue(float x, float height, std::string name);
    void callSceneResize();

public:

    b2World world;
    MainWindow(Model& model, QWidget *parent = nullptr);
    ~MainWindow();
    void createTreatment(std::string name, std::string imageLoc , int xLoc, int yLoc, int xDim, int yDim);

private slots:


    void on_toggleCanDrop_clicked();

private:
    int expandedX = 500;
    void setSceneSize();
    Model *mainModel;
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;

    b2Body *body;

    QGraphicsPixmapItem *pic;
    QTimer *timer;
    QPixmap image;

    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    void toggleTreatmentCanDrop(bool checked);

};
#endif // MAINWINDOW_H
