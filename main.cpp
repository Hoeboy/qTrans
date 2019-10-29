#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include "mainwindow.h"
#include "qtranswindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
