#ifndef SEVENPARAMETERDIALOG_H
#define SEVENPARAMETERDIALOG_H

#include <QDialog>
#include <QValidator>
#include "SurveyMath/SurveyMath.h"
#include "SurveyMath/BasePoint.h"
#include "SurveyMath/CoordinateTransformation.h"
#include "SurveyMath/Ellipsoid.h"
#include "SurveyMath/Projection.h"
#include "SurveyMath/SevenParameter.h"
using namespace SurveyMath;

namespace Ui {
class SevenParameterDialog;
}

class SevenParameterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SevenParameterDialog(QWidget *parent = nullptr);
    ~SevenParameterDialog();

private slots:

    void on_pushButton_Caculate_clicked();

    void on_pushButton_Close_clicked();

    void on_pushButton_Target_File_clicked();

    void on_pushButton_View_clicked();

    void on_pushButton_Enter_clicked();

    void on_pushButton_Cancel_clicked();

    void on_comboBox_CaculateMethod_activated(const QString &arg1);

private:
    Ui::SevenParameterDialog *ui;
    Projection prj_bj54;
    Projection prj_xian80;
    Projection prj_cscg2000;
    Projection prj_wgs84;
    Projection sourcePrjoject;
    Projection targetPrjoject;
    SevenParameter sevenParameter;
    QString ProjectName[4];
    QString CoordinateType[2];
    bool isCaculate;

    QVector<BasePoint> sourcePointVector;
    QVector<BasePoint> targetPointVector;
    QVector<BasePoint> inputPointVector;
    QVector<BasePoint> outputPointVector;

    QRegExpValidator *pRevLongitude;//正则表达式经度匹配

};

#endif // SEVENPARAMETERDIALOG_H
