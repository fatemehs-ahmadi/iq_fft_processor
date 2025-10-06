#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;                   //Create the main window instance
    w.show();                       //displays the main window on screen
    return app.exec();              //Enter Qt's event loop and wait for user interaction
}
