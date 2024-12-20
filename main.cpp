#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    LoginWindow ww;
    w.resize(1600, 800);
    //w.show();
    ww.show();
    return a.exec();
}
