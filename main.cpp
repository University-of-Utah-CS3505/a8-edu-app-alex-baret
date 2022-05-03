/**
 * 3505 A8 Education App
 * Team Fresca (Alex, Alivia, Ian, Joey)
 * 5/3/22
 *
 * main.cpp
 *
 **/

#include "mainwindow.h"
#include "model.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model model;
    MainWindow w(model);
    w.show();
    return a.exec();
}
