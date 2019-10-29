#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QSharedPointer>
#include <QMainWindow>
#include "dialogchangeangle.h"
#include "viewsevenparameterdialog.h"
#include "dialogcoordinatetokml.h"
#include "dialogchangezone.h"
#include "dialogcoordinatedirectsolution.h"
#include "dialogcoordinateinvertsolution.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_caculatesevenpara_triggered();

    void on_action_changeangle_triggered();

    void on_action_coordinatetokml_triggered();

    void on_action_changezone_triggered();

    void on_action_coordinatedirectsolution_triggered();

    void on_action_coordinateinversionsolution_triggered();

private:
    Ui::MainWindow *ui;
    QSharedPointer<DialogChangeAngle> dialogChangeAngle;
    QSharedPointer<ViewSevenParameterDialog> viewsevenParameterDialog;
    QSharedPointer<DialogCoordinateToKml> dialogCoordinateToKml;
    QSharedPointer<DialogChangeZone> dialogChangeZone;
    QSharedPointer<DialogCoordinateDirectsolution> dialogCoordinateDirectsolution;
    QSharedPointer<DialogCoordinateInvertsolution> dialogCoordinateInvertsolution;

};

#endif // MAINWINDOW_H
