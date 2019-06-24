#include "mainwindow.h"
#include "ecu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    ECU ecu;
    //ecu.show();

    return a.exec();
}
