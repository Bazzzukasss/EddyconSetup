#include "mainwindow.h"
#include <QApplication>
#include <UDPClient_EddyconC.h>
#include <QStyle>
#include <QtGui>
#include "TThread.h"
#include <QDebug>
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
