#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSharedPointer>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_caculatesevenpara_triggered()
{
    viewsevenParameterDialog = QSharedPointer<ViewSevenParameterDialog>(new ViewSevenParameterDialog(this));
    viewsevenParameterDialog->show();
    viewsevenParameterDialog->setFixedSize(370,455);
}

void MainWindow::on_action_changeangle_triggered()
{
    dialogChangeAngle = QSharedPointer<DialogChangeAngle>(new DialogChangeAngle(this)) ;
    dialogChangeAngle->show();
    dialogChangeAngle->setFixedSize(490,350);

}

void MainWindow::on_action_coordinatetokml_triggered()
{
    dialogCoordinateToKml = QSharedPointer<DialogCoordinateToKml>(new DialogCoordinateToKml(this)) ;
    dialogCoordinateToKml->show();
    dialogCoordinateToKml->setFixedSize(560,195);
}

void MainWindow::on_action_changezone_triggered()
{
    dialogChangeZone = QSharedPointer<DialogChangeZone>(new DialogChangeZone(this)) ;
    dialogChangeZone->show();
    dialogChangeZone->setFixedSize(450,320);

}

void MainWindow::on_action_coordinatedirectsolution_triggered()
{
    dialogCoordinateDirectsolution = QSharedPointer<DialogCoordinateDirectsolution>(new DialogCoordinateDirectsolution(this)) ;
    dialogCoordinateDirectsolution->show();
    dialogCoordinateDirectsolution->setFixedSize(550,315);
}

void MainWindow::on_action_coordinateinversionsolution_triggered()
{
    dialogCoordinateInvertsolution = QSharedPointer<DialogCoordinateInvertsolution>(new DialogCoordinateInvertsolution(this)) ;
    dialogCoordinateInvertsolution->show();
    dialogCoordinateInvertsolution->setFixedSize(550,315);
}
