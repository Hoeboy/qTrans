#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include "ui_dialogchangezone.h"
#include "dialogchangezone.h"
DialogChangeZone::DialogChangeZone(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChangeZone)
{
    ui->setupUi(this);
    Ellipsoid ell_wgs84(6378137.0, 1/298.257223563);
    Ellipsoid ell_bj54(6378245.0, 1/298.3);
    Ellipsoid ell_xian80(6378140.0, 1/298.257);
    Ellipsoid ell_cscg2000(6378137.0, 1/298.257222101);
    prj_bj54.SetDatum(ell_bj54);
    prj_utm.SetDatum(ell_wgs84);
    prj_xian80.SetDatum(ell_xian80);
    prj_cscg2000.SetDatum(ell_cscg2000);


    ProjectName[0]=QString::fromLocal8Bit("国家2000");
    ProjectName[1]=QString::fromLocal8Bit("北京54");
    ProjectName[2]=QString::fromLocal8Bit("西安80");
    ProjectName[3]=QString::fromLocal8Bit("WGS84");
    ProjectName[4]=QString::fromLocal8Bit("UTM");

    CoordinateType[0]=QString::fromLocal8Bit("大地坐标");
    CoordinateType[1]=QString::fromLocal8Bit("平面坐标");


    pRevZone = new  QRegExpValidator(QRegExp("^(([1-9]|[1-5][0-9]|(60)))([sSnN]?)$")) ;//正则表达式带号匹配1N(S)-60N(S)
    pRevLatitude = new  QRegExpValidator(QRegExp("^-?(90|[1-8]?\\d(\\.\\d{1,10})?)$")) ;//正则表达式纬度匹配
    pRevLongitude = new  QRegExpValidator(QRegExp("^-?(180|1?[0-7]?\\d(\\.\\d{1,10})?)$")) ;//正则表达式经度匹配
    pRevDecimal = new  QRegExpValidator(QRegExp("^([0-9]{1,}[.][0-9]*)$")) ;//正则表达式小数匹配
    ui->lineEdit_ChangeZone_BeforeMiddleLon->setValidator(pRevLongitude);
    ui->lineEdit_ChangeZone_AfterMiddleLon->setValidator(pRevLongitude);
    ui->lineEdit_ChangeZone_BeforeX->setValidator(pRevDecimal);
    ui->lineEdit_ChangeZone_BeforeY->setValidator(pRevDecimal);
    ui->lineEdit_ChangeZone_AfterX->setValidator(pRevDecimal);
    ui->lineEdit_ChangeZone_AfterY->setValidator(pRevDecimal);
     UserFilePath = ".";
}

DialogChangeZone::~DialogChangeZone()
{
    delete ui;
}

void DialogChangeZone::on_pushButton_ChangeZone_Point_clicked()
{
    BasePoint sourcePoint;
    BasePoint targetPoint;
    int iDecimalsXY=4;
    sourcePoint.x =ui->lineEdit_ChangeZone_BeforeX->text().toDouble();
    sourcePoint.y =ui->lineEdit_ChangeZone_BeforeY->text().toDouble();
    sourcePoint.z =100;
    targetPoint = ChangeZone(sourcePoint);
    ui->lineEdit_ChangeZone_AfterX->setText(QString::number(targetPoint.x,4,iDecimalsXY));
    ui->lineEdit_ChangeZone_AfterY->setText(QString::number(targetPoint.y,4,iDecimalsXY));
}
BasePoint DialogChangeZone::ChangeZone(BasePoint inputPoint)
{
    Projection currentProject;

    if(ui->comboBox_ChangeZone_Project->currentIndex() == 0)//国家2000
    {
        currentProject = prj_cscg2000;
    }
    if(ui->comboBox_ChangeZone_Project->currentIndex() == 1)//北京54
    {
         currentProject = prj_bj54;
    }
    if(ui->comboBox_ChangeZone_Project->currentIndex() == 2)//西安80
    {
         currentProject = prj_xian80;
    }

    double beforeMiddleLon = DMSToRadian(ui->lineEdit_ChangeZone_BeforeMiddleLon->text().toDouble());
    double afterMiddleLon = DMSToRadian(ui->lineEdit_ChangeZone_AfterMiddleLon->text().toDouble());
    double falseEast = ui->lineEdit_ChangeZone_FalseEast->text().toDouble();
    double falseNorth = ui->lineEdit_ChangeZone_FalseNorth->text().toDouble();

    currentProject.SetFalseEasting(falseEast);
    currentProject.SetFalseNorthing(falseNorth);

    //换带前中央子午线
    currentProject.SetCentralMeridian(beforeMiddleLon);
    //将网格坐标转换为大地坐标,高斯坐标反算
    BasePoint tempPoint = CoordinateTransformation::InverseGauss(currentProject,inputPoint);
    //换带后中央子午线
    currentProject.SetCentralMeridian(afterMiddleLon);
    //改变中央子午线，将大地坐标转换为网格坐标,高斯坐标正算
    BasePoint outputPoint = CoordinateTransformation::PositiveGauss(currentProject,tempPoint);
    return outputPoint;
}

void DialogChangeZone::on_pushButton_ChangeZone_SelectFile_clicked()
{
    QString csvFileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择需转换的文件"), ".", QString::fromLocal8Bit("逗号分隔符文件(*.csv)"), 0);
    if (!csvFileName.isEmpty())
    {
       ui->lineEdit_ChangeZone_FilePath->setText(csvFileName);
       QFileInfo fileInfo (csvFileName);
       UserFilePath = fileInfo.filePath().left( fileInfo.filePath().size() -4) + "Z" ;
    }
    else
    {
       return;
    }
}

void DialogChangeZone::on_pushButton_ChangeZone_File_clicked()
{
    QFile sourceFile(ui->lineEdit_ChangeZone_FilePath->text());
    QTextStream inSourceFile(&sourceFile);
    QVector<SurveyPoint> inputPointVector;
    if(!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       return;
    }
    //读取源坐标
    while(!inSourceFile.atEnd())
    {
        QStringList lineList;
        QString strLine = inSourceFile.readLine();
        lineList = strLine.split(",", QString::SkipEmptyParts);
        if(lineList.size() < 3)
        {
            continue;
        }
        SurveyPoint tP;
        tP.PointName = lineList.at(0);
        tP.x=lineList.at(1).toDouble();
        tP.y=lineList.at(2).toDouble();
        if(lineList.size() == 4)
        {
             tP.z=lineList.at(3).toDouble();
        }
        else
        {
            tP.z = 0;
        }
        inputPointVector.push_back(tP);

    }
    QString csvfileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("请输入文件名"), "/", QString::fromLocal8Bit("csv文件(*.csv)"),0);


    QFile csvFile(csvfileName);
    if(!csvFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return ;
    }

    QTextStream txtOutput(&csvFile);
    for(int i=0;i<inputPointVector.size();i++)
    {
        BasePoint targetPoint = ChangeZone(inputPointVector[i]);
        txtOutput<<inputPointVector[i].PointName
                 <<qSetRealNumberPrecision(3)<<fixed
                 <<","<<targetPoint.x
                 <<","<<targetPoint.y
                 <<","<<targetPoint.z
                 <<endl;
    }
    csvFile.close();
}
