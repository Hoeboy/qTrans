#ifndef VIEWSEVENPARAMETERDIALOG_H
#define VIEWSEVENPARAMETERDIALOG_H

#include <QDialog>
#include <QValidator>
#include "sevenparameterdialog.h"
#include "ui_viewsevenparameterdialog.h"


namespace Ui {
class ViewSevenParameterDialog;
}

class ViewSevenParameterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewSevenParameterDialog(QWidget *parent = nullptr);
    ~ViewSevenParameterDialog();

private slots:

    void on_pushButton_Caculate_clicked();

    void on_pushButton_Close_clicked();

    void on_pushButton_Save_clicked();

    void on_pushButton_Input_clicked();

    void on_pushButton_Apply_clicked();

private:
    Ui::ViewSevenParameterDialog *ui;
    QTableWidgetItem *itemType;
    QTableWidgetItem *itemProject;
    QTableWidgetItem *itemMiddleLine;
    QTableWidgetItem *itemFalseEast;
    QTableWidgetItem *itemFalseNorth;
    QTableWidgetItem *itemDX;
    QTableWidgetItem *itemDY;
    QTableWidgetItem *itemDZ;
    QTableWidgetItem *itemRX;
    QTableWidgetItem *itemRY;
    QTableWidgetItem *itemRZ;
    QTableWidgetItem *itemPPM;


};

#endif // VIEWSEVENPARAMETERDIALOG_H
