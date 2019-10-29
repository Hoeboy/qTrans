#ifndef DIALOGCHANGEANGLE_H
#define DIALOGCHANGEANGLE_H

#include "SurveyMath/SurveyMath.h"
#include "SurveyMath/BasePoint.h"
#include "SurveyMath/CoordinateTransformation.h"
#include "SurveyMath/Ellipsoid.h"
#include "SurveyMath/Projection.h"
#include "SurveyMath/SevenParameter.h"
#include "SurveyPoint.h"
#include <QDialog>
#include <QValidator>
#include <QFileInfo>
using namespace SurveyMath;
namespace Ui {
class DialogChangeAngle;
}

class DialogChangeAngle : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChangeAngle(QWidget *parent = nullptr);
    ~DialogChangeAngle();

private slots:


    void on_pushButton_AngleTran_Point_clicked();

    void on_comboBox_AngleTran_Source_currentIndexChanged(const QString &arg1);

    void on_pushButton_Another_AngleTran_SelectFile_clicked();

    void on_pushButton_AngleTran_File_clicked();

private:
    Ui::DialogChangeAngle *ui;
    QString UserFilePath;
private:
    double ChangeAngle(double inputAngle);


};
#endif // DIALOGCHANGEANGLE_H
