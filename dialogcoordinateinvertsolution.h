#ifndef DIALOGCOORDINATEINVERTSOLUTION_H
#define DIALOGCOORDINATEINVERTSOLUTION_H

#include "SurveyMath/SurveyMath.h"
#include "SurveyMath/BasePoint.h"
#include "SurveyMath/CoordinateTransformation.h"
#include "SurveyMath/Ellipsoid.h"
#include "SurveyMath/Projection.h"
#include "SurveyMath/SevenParameter.h"
#include "SurveyPoint.h"
#include <QFileDialog>
#include <QValidator>
#include <QFileInfo>
using namespace SurveyMath;
namespace Ui {
class DialogCoordinateInvertsolution;
}

class DialogCoordinateInvertsolution : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCoordinateInvertsolution(QWidget *parent = nullptr);
    ~DialogCoordinateInvertsolution();

private slots:

    void on_pushButton_Trans_TranCoor_clicked();

    void on_comboBox_Trans_Project_currentIndexChanged(int index);

    void on_pushButton_Tans_SelectFile_clicked();

    void on_lineEdit_Trans_MiddleLon_editingFinished();

    void on_pushButton_Tans_TansFile_clicked();

private:
    Ui::DialogCoordinateInvertsolution *ui;
    Projection prj_bj54;
    Projection prj_xian80;
    Projection prj_cscg2000;
    Projection prj_wgs84;
    Projection prj_utm;
    Projection sourcePrjoject;
    Projection targetPrjoject;
    SevenParameter sevenParameter;
    QString ProjectName[5];
    QString CoordinateType[2];
    int currentProjectIndex;
    BasePoint TransCoor(BasePoint inputPoint);
    BasePoint TransCoorFromSevenParam(BasePoint inputPoint);
    QRegExpValidator *pRevZone;//正则表达式带号匹配
    QRegExpValidator *pRevLatitude;//正则表达式纬度匹配
    QRegExpValidator *pRevLongitude;//正则表达式经度匹配
    QRegExpValidator *pRevDecimal;//正则表达式小数匹配
    QString UserFilePath;

};

#endif // DIALOGCOORDINATEINVERTSOLUTION_H
