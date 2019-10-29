#ifndef QTRANSWINDOW_H
#define QTRANSWINDOW_H

#include "SurveyMath/SurveyMath.h"
#include "SurveyMath/BasePoint.h"
#include "SurveyMath/CoordinateTransformation.h"
#include "SurveyMath/Ellipsoid.h"
#include "SurveyMath/Projection.h"
#include "SurveyMath/SevenParameter.h"
#include "SurveyPoint.h"
#include <QMainWindow>
#include <QValidator>
#include <QFileInfo>
using namespace SurveyMath;
namespace Ui {
class qTransWindow;
}

class qTransWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit qTransWindow(QWidget *parent = nullptr);
    ~qTransWindow();

private slots:
    void on_comboBox_AngleTran_Source_currentIndexChanged(const QString &arg1);

    void on_pushButton_AngleTran_Point_clicked();

    void on_pushButton_Another_AngleTran_SelectFile_clicked();

    void on_comboBox_Trans_Source_CoorType_currentIndexChanged(int index);

    void on_pushButton_Trans_TranCoor_clicked();

    void on_comboBox_Trans_Project_currentIndexChanged(int index);

    void on_pushButton_Tans_SelectFile_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_checkBox_Trans_UserServenParm_stateChanged(int arg1);

    void on_menu_seven_action_caculate_triggered();

    void on_checkBox_Trans_UserServenParm_clicked();

    void on_lineEdit_Trans_MiddleLon_editingFinished();

    void on_pushButton_Trans_ClearData_clicked();
    
    void on_pushButton_ChangeZone_clicked();

    void on_pushButton_AngleTran_File_clicked();

    void on_pushButton_ChangeZone_SelectFile_clicked();

    void on_pushButton_ChangeZone_File_clicked();

    void on_comboBox_ChangeZone_Project_activated(int index);

    void on_comboBox_ChangeZone_Project_activated(const QString &arg1);

    void on_pushButton_Tans_TansFile_clicked();

    void on_comboBox_Trans_Project_activated(int index);

    void on_comboBox_Trans_Project_activated(const QString &arg1);

    void on_comboBox_AngleTran_Source_activated(int index);

    void on_comboBox_AngleTran_Source_activated(const QString &arg1);

private:
    Ui::qTransWindow *ui;  
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
    BasePoint TransCoor(BasePoint inputPoint);
    BasePoint TransCoorFromSevenParam(BasePoint inputPoint);
    BasePoint ChangeZone(BasePoint inputPoint);
    double ChangeAngle(double inputAngle);
    QRegExpValidator *pRevZone;//正则表达式带号匹配
    QRegExpValidator *pRevLatitude;//正则表达式纬度匹配
    QRegExpValidator *pRevLongitude;//正则表达式经度匹配
    QRegExpValidator *pRevDecimal;//正则表达式小数匹配
    QString UserFilePath;

};

#endif // QTRANSWINDOW_H
