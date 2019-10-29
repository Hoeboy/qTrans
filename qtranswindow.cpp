#include "qtranswindow.h"
#include "ui_qtranswindow.h"
#include "sevenparameterdialog.h"
#include "sevenparameterhassistant.h"
#include "selectcolumndialog.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
qTransWindow::qTransWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::qTransWindow)
{
    setFixedSize(640,425); // 禁止改变窗口大小。
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

    ui->label_Trans_Zone->setVisible(false);
    ui->lineEdit_Trans_Zone->setVisible(false);

    pRevZone = new  QRegExpValidator(QRegExp("^(([1-9]|[1-5][0-9]|(60)))([sSnN]?)$")) ;//正则表达式带号匹配1N(S)-60N(S)
    pRevLatitude = new  QRegExpValidator(QRegExp("^-?(90|[1-8]?\\d(\\.\\d{1,10})?)$")) ;//正则表达式纬度匹配
    pRevLongitude = new  QRegExpValidator(QRegExp("^-?(180|1?[0-7]?\\d(\\.\\d{1,10})?)$")) ;//正则表达式经度匹配
    pRevDecimal = new  QRegExpValidator(QRegExp("^([0-9]{1,}[.][0-9]*)$")) ;//正则表达式小数匹配
    ui->lineEdit_Trans_Zone->setValidator(pRevZone);
    ui->lineEdit_Trans_MiddleLon->setValidator(pRevLongitude);
    ui->lineEdit_Trans_FalseEast->setValidator(pRevDecimal);
    ui->lineEdit_Trans_FalseNorth->setValidator(pRevDecimal);
    ui->lineEdit_ChangeZone_BeforeMiddleLon->setValidator(pRevLongitude);
    ui->lineEdit_ChangeZone_AfterMiddleLon->setValidator(pRevLongitude);
    ui->lineEdit_ChangeZone_BeforeX->setValidator(pRevDecimal);
    ui->lineEdit_ChangeZone_BeforeY->setValidator(pRevDecimal);
    ui->lineEdit_ChangeZone_AfterX->setValidator(pRevDecimal);
    ui->lineEdit_ChangeZone_AfterY->setValidator(pRevDecimal);
    ui->lineEdit_KML_MiddleLon->setValidator(pRevLongitude);
    ui->lineEdit_KML_FalseEast->setValidator(pRevDecimal);
    ui->lineEdit_KML_FalseNorth->setValidator(pRevDecimal);
    ui->lineEdit_AngleTran_Soruce->setValidator(pRevDecimal);
    ui->lineEdit_AngleTran_Target->setValidator(pRevDecimal);
    ui->lineEdit_Trans_Source_X->setValidator(pRevLatitude);
    ui->lineEdit_Trans_Source_Y->setValidator(pRevLongitude);
    ui->lineEdit_Trans_Source_Z->setValidator(pRevDecimal);
    ui->lineEdit_Trans_Target_X->setValidator(pRevDecimal);
    ui->lineEdit_Trans_Target_Y->setValidator(pRevDecimal);
    ui->lineEdit_Trans_Target_Z->setValidator(pRevDecimal);
    UserFilePath = ".";
}

qTransWindow::~qTransWindow()
{
    delete ui;
    delete pRevZone;
    delete pRevLatitude;
    delete pRevLongitude;
    delete pRevDecimal;
}

void qTransWindow::on_comboBox_AngleTran_Source_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_AngleTran_Target->clear();
    QString strAngle[4];
    strAngle[0]=QString::fromLocal8Bit("十进制度");
    strAngle[1]=QString::fromLocal8Bit("度分秒");
    strAngle[2]=QString::fromLocal8Bit("弧度");
    strAngle[3]=QString::fromLocal8Bit("百分度");
    for(int i=0;i<4;i++)
    {
        if(strAngle[i]!=arg1)
        {
            ui->comboBox_AngleTran_Target->addItem(strAngle[i]);

        }
    }
}
//批量角度单位格式转换-选择文件
void qTransWindow::on_pushButton_Another_AngleTran_SelectFile_clicked()
{
    QString csvFileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择源文件"), UserFilePath, QString::fromLocal8Bit("文本文件(*.csv)"), 0);
    if (!csvFileName.isEmpty())
    {
        ui->lineEdit_AngleTran_FileName->setText(csvFileName);
    }
    else
    {
        return;
    }
    QFileInfo fileInfo (csvFileName);
    UserFilePath = fileInfo.filePath();
}
//批量角度单位格式转换-批量转换
void qTransWindow::on_pushButton_AngleTran_File_clicked()
{
    QFile sourceFile(ui->lineEdit_AngleTran_FileName->text());
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
    QString csvfileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("请输入文件名"), UserFilePath, QString::fromLocal8Bit("csv文件(*.csv)"),0);
    QFileInfo fileInfo (csvfileName);
    UserFilePath = fileInfo.filePath();
    QFile csvFile(csvfileName);
    if(!csvFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return ;
    }

    QTextStream txtOutput(&csvFile);
    for(int i=0;i<inputPointVector.size();i++)
    {
        BasePoint targetPoint;
        targetPoint.x = ChangeAngle(inputPointVector[i].x);
        targetPoint.y = ChangeAngle(inputPointVector[i].y);
        targetPoint.z = inputPointVector[i].z;
        txtOutput<<inputPointVector[i].PointName
                 <<qSetRealNumberPrecision(9)<<fixed
                 <<","<<targetPoint.x
                 <<","<<targetPoint.y
                 <<","<<targetPoint.z
                 <<endl;
    }
    csvFile.close();
}

//单点角度单位格式转换
double qTransWindow::ChangeAngle(double inputAngle)
{
    double dSource=inputAngle;
    double dTarget = 0;
    QString qStrSource=ui->comboBox_AngleTran_Source->currentText();
    QString qStrTarget=ui->comboBox_AngleTran_Target->currentText();
    QString strAngle[4];
    strAngle[0]=QString::fromLocal8Bit("十进制度");
    strAngle[1]=QString::fromLocal8Bit("度分秒");
    strAngle[2]=QString::fromLocal8Bit("弧度");
    strAngle[3]=QString::fromLocal8Bit("百分度");

    if(qStrSource == strAngle[0] )
    {
        if(qStrTarget == strAngle[1])//十进制度转度分秒
        {
           dTarget = DegreeToDMS(dSource);
        }
        if(qStrTarget == strAngle[2])//十进制度转弧度
        {
           dTarget = DegreeToRadian(dSource);
        }
        if(qStrTarget == strAngle[3])//十进制度转百分度
        {
           dTarget = DegreeToGradian(dSource);
        }
    }
    if(qStrSource == strAngle[1] )
    {
        if(qStrTarget == strAngle[0])//度分秒转十进制度
        {
           dTarget = DMSToDegree(dSource);
        }
        if(qStrTarget == strAngle[2])//度分秒度转弧度
        {
           dTarget = DMSToRadian(dSource);
        }
        if(qStrTarget == strAngle[3])//度分秒转百分度
        {
           double dDeg=DMSToDegree(dSource);//先把度分秒转为度
           dTarget = DegreeToGradian(dDeg);
        }
    }
    if(qStrSource == strAngle[2] )
    {
        if(qStrTarget == strAngle[0])//弧度转十进制度
        {
            dTarget = RadianToDegree(dSource);

        }
        if(qStrTarget == strAngle[1])//弧度转度分秒
        {
            dTarget = RadianToDMS(dSource);

        }
        if(qStrTarget == strAngle[3])//弧度转百分度
        {
            double dDeg=RadianToDegree(dSource);//先把弧度转为度
            dTarget = DegreeToGradian(dDeg);

        }
    }
    if(qStrSource == strAngle[3] )
    {
        if(qStrTarget == strAngle[0])//百分度转十进制度
        {
            dTarget = GradianToDegree(dSource);

        }
        if(qStrTarget == strAngle[1])//百分度转度分秒
        {
            double dDeg=GradianToDegree(dSource);//先把百分度转为度
            dTarget =DegreeToDMS(dDeg) ;
        }
        if(qStrTarget == strAngle[2])//百分度转弧度
        {
            double dDeg=GradianToDegree(dSource);//先把百分度转为度
            dTarget =DegreeToRadian(dDeg) ;
        }
    }
    return dTarget;
}


//单个角度单位格式转换
void qTransWindow::on_pushButton_AngleTran_Point_clicked()
{
    double dSource=ui->lineEdit_AngleTran_Soruce->text().toDouble();
    double dTarget = ChangeAngle(dSource);
    ui->lineEdit_AngleTran_Target->setText(QString::number(dTarget,14,9));
}


void qTransWindow::on_comboBox_Trans_Source_CoorType_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->label_Trans_Soruce_X->setText(QString::fromLocal8Bit("纬度："));
        ui->label_Trans_Soruce_Y->setText(QString::fromLocal8Bit("经度："));
        ui->label_Trans_Soruce_Z->setText(QString::fromLocal8Bit("椭球高："));
        ui->label_Trans_Target_X->setText(QString::fromLocal8Bit("北："));
        ui->label_Trans_Target_Y->setText(QString::fromLocal8Bit("东："));
        ui->label_Trans_Target_Z->setText(QString::fromLocal8Bit("高程："));
        ui->label_Trans_Zone->setVisible(false);
        ui->lineEdit_Trans_Zone->setVisible(false);
        ui->lineEdit_Trans_Source_X->setValidator(pRevLatitude);
        ui->lineEdit_Trans_Source_Y->setValidator(pRevLongitude);
        ui->lineEdit_Trans_Target_X->setValidator(pRevDecimal);
        ui->lineEdit_Trans_Target_Y->setValidator(pRevDecimal);

        break;
    case 1:
        ui->label_Trans_Soruce_X->setText(QString::fromLocal8Bit("北："));
        ui->label_Trans_Soruce_Y->setText(QString::fromLocal8Bit("东："));
        ui->label_Trans_Soruce_Z->setText(QString::fromLocal8Bit("高程："));
        ui->label_Trans_Target_X->setText(QString::fromLocal8Bit("纬度："));
        ui->label_Trans_Target_Y->setText(QString::fromLocal8Bit("经度："));
        ui->label_Trans_Target_Z->setText(QString::fromLocal8Bit("椭球高："));
        ui->lineEdit_Trans_Source_X->setValidator(pRevDecimal);
        ui->lineEdit_Trans_Source_Y->setValidator(pRevDecimal);
        ui->lineEdit_Trans_Target_X->setValidator(pRevLatitude);
        ui->lineEdit_Trans_Target_Y->setValidator(pRevLongitude);
        if(ui->comboBox_Trans_Project->currentText() == "UTM")//投影类型为UTM
         {
            ui->label_Trans_Zone->setVisible(true);
            ui->lineEdit_Trans_Zone->setVisible(true);
         }
        break;
    }
}

void qTransWindow::on_pushButton_Trans_TranCoor_clicked()
{
    BasePoint sourcePoint;
    BasePoint targetPoint;

    int iDecimalsXY=4;
    int iDecimalsZ=4;
    sourcePoint.z =ui->lineEdit_Trans_Source_Z->text().toDouble();
    if(ui->comboBox_Trans_Source_CoorType->currentText() == CoordinateType[0])//源坐标为大地坐标，目标坐标为平面
    {
        sourcePoint.x =DMSToRadian(ui->lineEdit_Trans_Source_X->text().toDouble());
        sourcePoint.y =DMSToRadian(ui->lineEdit_Trans_Source_Y->text().toDouble());
        if(ui->checkBox_Trans_UserServenParm->isChecked() == true)
        {
            targetPoint = TransCoorFromSevenParam(sourcePoint);
        }
        else
        {
           targetPoint = TransCoor(sourcePoint);
        }
    }
    if(ui->comboBox_Trans_Source_CoorType->currentText() == CoordinateType[1])//源坐标为平面坐标，目标坐标为大地坐标
    {
        iDecimalsXY = 10;
        sourcePoint.x = ui->lineEdit_Trans_Source_X->text().toDouble();
        sourcePoint.y = ui->lineEdit_Trans_Source_Y->text().toDouble();

       if(ui->comboBox_Trans_Project->currentText() == "UTM")//投影类型为UTM
        {
            if(ui->lineEdit_Trans_Zone->text().isEmpty())
            {
                QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("请输入带号（如12N,33S)!"), QMessageBox::Ok, QMessageBox::Ok);
                return;
            }
            if(ui->lineEdit_Trans_Zone->text().right(1) != "S" && ui->lineEdit_Trans_Zone->text().right(1) != "N")
            {
                QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("请输入正确的带号（如12N,33S)!"), QMessageBox::Ok, QMessageBox::Ok);
                return;
            }
        }
       BasePoint tP;
       if(ui->checkBox_Trans_UserServenParm->isChecked() == true)
       {

           tP = TransCoorFromSevenParam(sourcePoint);
       }
       else
       {
           tP = TransCoor(sourcePoint);
       }
       targetPoint.x = RadianToDMS(tP.x);
       targetPoint.y = RadianToDMS(tP.y);
       targetPoint.z = tP.z;
    }
    ui->lineEdit_Trans_Target_X->setText(QString::number(targetPoint.x,10,iDecimalsXY));
    ui->lineEdit_Trans_Target_Y->setText(QString::number(targetPoint.y,10,iDecimalsXY));
    ui->lineEdit_Trans_Target_Z->setText(QString::number(targetPoint.z,10,iDecimalsZ));
}

void qTransWindow::on_comboBox_Trans_Project_currentIndexChanged(int index)
{
    if(index == 4)
    {
        ui->lineEdit_Trans_MiddleLon->setEnabled(false);
        ui->lineEdit_Trans_FalseEast->setEnabled(false);
        ui->lineEdit_Trans_FalseNorth->setEnabled(false);
        if(ui->comboBox_Trans_Source_CoorType->currentText() == CoordinateType[1])//坐标类型为平面坐标
         {
            ui->label_Trans_Zone->setVisible(true);
            ui->lineEdit_Trans_Zone->setVisible(true);
         }
    }
    else
    {
        ui->lineEdit_Trans_MiddleLon->setEnabled(true);
        ui->lineEdit_Trans_FalseEast->setEnabled(true);
        ui->lineEdit_Trans_FalseNorth->setEnabled(true);
        ui->label_Trans_Zone->setVisible(false);
        ui->lineEdit_Trans_Zone->setVisible(false);

    }

}
//选择需要坐标转换的文件
void qTransWindow::on_pushButton_Tans_SelectFile_clicked()
{
    QString csvFileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择源文件"), UserFilePath, QString::fromLocal8Bit("文本文件(*.csv)"), 0);
    if (!csvFileName.isEmpty())
    {
        ui->lineEdit_Trans_FilePath->setText(csvFileName);
    }
    else
    {
        return;
    }
    QFileInfo fileInfo (csvFileName);
    UserFilePath = fileInfo.filePath();
}
//批量进行坐标转换
void qTransWindow::on_pushButton_Tans_TansFile_clicked()
{
    QFile sourceFile(ui->lineEdit_Trans_FilePath->text());
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
            qDebug()<<lineList.size();
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
    QString csvfileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("请输入文件名"), UserFilePath, QString::fromLocal8Bit("csv文件(*.csv)"),0);
    QFileInfo fileInfo (csvfileName);
    UserFilePath = fileInfo.filePath();
    QFile csvFile(csvfileName);
    if(!csvFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return ;
    }
    QTextStream txtOutput(&csvFile);

    for(int i=0;i<inputPointVector.size();i++)
    {
        BasePoint sourcePoint;
        BasePoint targetPoint;
        int iDecimalsXY=4;
        if(ui->comboBox_Trans_Source_CoorType->currentText() == CoordinateType[0])//源坐标为大地坐标，目标坐标为平面
        {

            sourcePoint.x =DMSToRadian(inputPointVector[i].x);
            sourcePoint.y =DMSToRadian(inputPointVector[i].y);
            if(ui->checkBox_Trans_UserServenParm->isChecked() == true)
            {
               targetPoint = TransCoorFromSevenParam(sourcePoint);
            }
            else
            {
               targetPoint = TransCoor(sourcePoint);
            }
        }
        if(ui->comboBox_Trans_Source_CoorType->currentText() == CoordinateType[1])//源坐标为平面坐标，目标坐标为大地坐标
        {
            iDecimalsXY = 10;
            sourcePoint.x =inputPointVector[i].x;
            sourcePoint.y =inputPointVector[i].y;

           if(ui->comboBox_Trans_Project->currentText() == "UTM")//投影类型为UTM
            {
                if(ui->lineEdit_Trans_Zone->text().isEmpty())
                {
                    QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("请输入带号（如12N,33S)!"), QMessageBox::Ok, QMessageBox::Ok);
                    return;
                }
                if(ui->lineEdit_Trans_Zone->text().right(1) != "S" && ui->lineEdit_Trans_Zone->text().right(1) != "N")
                {
                    QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("请输入正确的带号（如12N,33S)!"), QMessageBox::Ok, QMessageBox::Ok);
                    return;
                }
            }
           BasePoint tP;
           if(ui->checkBox_Trans_UserServenParm->isChecked() == true)
           {

               tP = TransCoorFromSevenParam(sourcePoint);
           }
           else
           {
               tP = TransCoor(sourcePoint);
           }
           targetPoint.x = RadianToDMS(tP.x);
           targetPoint.y = RadianToDMS(tP.y);
           targetPoint.z = tP.z;
        }
        txtOutput<<inputPointVector[i].PointName
                 <<qSetRealNumberPrecision(iDecimalsXY)<<fixed
                 <<","<<targetPoint.x
                 <<","<<targetPoint.y
                 <<","<<targetPoint.z
                 <<endl;
    }
    csvFile.close();
}
BasePoint qTransWindow::TransCoor(BasePoint inputPoint)
{
    Projection currentProject;
    BasePoint outputPoint;
    if(ui->comboBox_Trans_Project->currentText()==ProjectName[0])//国家2000
    {
        currentProject = prj_cscg2000;
    }
    if(ui->comboBox_Trans_Project->currentText()==ProjectName[1])//北京54
    {
         currentProject = prj_bj54;
    }
    if(ui->comboBox_Trans_Project->currentText()==ProjectName[2])//西安80
    {
         currentProject = prj_xian80;
    }
    if(ui->comboBox_Trans_Project->currentText()==ProjectName[3])//wgs84
    {
        currentProject = prj_wgs84;
    }
    if(ui->comboBox_Trans_Project->currentText()==ProjectName[4])//UTM
    {
        currentProject = prj_utm;
    }

    double middleLon = DMSToRadian(ui->lineEdit_Trans_MiddleLon->text().toDouble());
    double falseEast = ui->lineEdit_Trans_FalseEast->text().toDouble();
    double falseNorth = ui->lineEdit_Trans_FalseNorth->text().toDouble();

    currentProject.SetCentralMeridian(middleLon);
    currentProject.SetFalseEasting(falseEast);
    currentProject.SetFalseNorthing(falseNorth);

    if(ui->comboBox_Trans_Source_CoorType->currentText() == CoordinateType[0])//源坐标为大地坐标
    {
        if(ui->comboBox_Trans_Project->currentText() == "UTM")//投影类型为UTM
        {

            outputPoint = CoordinateTransformation::PositiveUTM(inputPoint);
        }
        else
        {
            outputPoint = CoordinateTransformation::PositiveGauss(currentProject,inputPoint);
        }
    }
    if(ui->comboBox_Trans_Source_CoorType->currentText() == CoordinateType[1])//源坐标为平面坐标
    {
        if(ui->comboBox_Trans_Project->currentText() == "UTM")//投影类型为UTM
        {
            bool isSouth;
            int iSize = ui->lineEdit_Trans_Zone->text().size();
            int iZone = ui->lineEdit_Trans_Zone->text().left(iSize - 1).toInt();
            if(ui->lineEdit_Trans_Zone->text().right(1).toUpper() == "S")
            {
                isSouth = true;
            }
            else
            {
                isSouth = false;
            }
            outputPoint = CoordinateTransformation::InverseUTM(iZone,isSouth,inputPoint);
        }
        else
        {
            outputPoint = CoordinateTransformation::InverseGauss(currentProject,inputPoint);
        }
    }
    return outputPoint;
}
//七参数转换，其中角度单位为弧度
BasePoint qTransWindow::TransCoorFromSevenParam(BasePoint inputPoint)
{
    BasePoint outputPoint;
    sourcePrjoject =SevenParameterhAssistant::sourcePrjoject;
    targetPrjoject = SevenParameterhAssistant::targetPrjoject;
    if(SevenParameterhAssistant::Type == 0)
    {
        if(ui->comboBox_Trans_Source_CoorType->currentText() == CoordinateType[0])//源坐标为大地坐标
        {
            //将输入点大地坐标转换为平面坐标
            GeocentricCoordinate gcp_s = CoordinateTransformation::PositiveGauss(sourcePrjoject, inputPoint);
            //利用7参数将输入点平面坐标转换成输出点平面坐标
           outputPoint = SevenParameterhAssistant::sevenParameter.Transform(gcp_s);
        }
        if(ui->comboBox_Trans_Source_CoorType->currentText() == CoordinateType[1])//源坐标为平面坐标
        {
             //利用7参数将输入点平面坐标转换成输出点所在坐标系的平面坐标
            GeocentricCoordinate gcp_t = SevenParameterhAssistant::sevenParameter.InverseTransform(inputPoint);
            //将输出点平面坐标转换为大地坐标
            outputPoint = CoordinateTransformation::InverseGauss(sourcePrjoject, gcp_t);
        }
    }
    else
    {
        if(ui->comboBox_Trans_Source_CoorType->currentText() == CoordinateType[0])//源坐标为大地坐标
        {
            //将输入点大地坐标转换为地心坐标
            GeocentricCoordinate gcp_s = CoordinateTransformation::GeodeticToGeocentric(sourcePrjoject, inputPoint);
            //利用7参数将输入点地心坐标转换成输出点所在坐标系的地心坐标
            GeocentricCoordinate gcp_t = SevenParameterhAssistant::sevenParameter.Transform(gcp_s);
            //将输出点地心坐标转换为大地坐标
            GeodeticCoordinate gep_t = CoordinateTransformation::GeocentricToGeodetic(targetPrjoject, gcp_t);
            //将输出点大地坐标转换为平面坐标
            outputPoint = CoordinateTransformation::PositiveGauss(targetPrjoject, gep_t);

        }
        if(ui->comboBox_Trans_Source_CoorType->currentText() == CoordinateType[1])//源坐标为平面坐标
        {
            //将输入点平面坐标转换为大地坐标
            GeodeticCoordinate gep_s = CoordinateTransformation::InverseGauss(targetPrjoject, inputPoint);
            //将输入点大地坐标转换为地心坐标
            GeocentricCoordinate gcp_s = CoordinateTransformation::GeodeticToGeocentric(targetPrjoject, gep_s);
            //利用7参数将输入点地心坐标转换成输出点所在坐标系的地心坐标
            GeocentricCoordinate gcp_t = SevenParameterhAssistant::sevenParameter.InverseTransform(gcp_s);
            //将输出点地心坐标转换为大地坐标
            outputPoint = CoordinateTransformation::GeocentricToGeodetic(sourcePrjoject, gcp_t);
        }
    }


    return outputPoint;
}

BasePoint qTransWindow::ChangeZone(BasePoint inputPoint)
{
    Projection currentProject;

    if(ui->comboBox_ChangeZone_Project->currentIndex() == 0)//国家2000
    {
        currentProject = prj_cscg2000;
        qDebug()<<currentProject.GetDatum().GetLongAxis();
    }
    if(ui->comboBox_ChangeZone_Project->currentIndex() == 1)//北京54
    {
         currentProject = prj_bj54;
                 qDebug()<<currentProject.GetDatum().GetLongAxis();
    }
    if(ui->comboBox_ChangeZone_Project->currentIndex() == 2)//西安80
    {
         currentProject = prj_xian80;
                 qDebug()<<currentProject.GetDatum().GetLongAxis();
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
/*
void qTransWindow::on_pushButton_Seven_Input_clicked()
{

}*/

void qTransWindow::on_tabWidget_currentChanged(int index)
{
    ui->statusBar->showMessage(QString::fromLocal8Bit("坐标文件格式为“点号,北,东,高”"));
}



void qTransWindow::on_checkBox_Trans_UserServenParm_stateChanged(int arg1)
{
    QString strCoorTypeG=QString::fromLocal8Bit("大地坐标");
    QString strCoorTypeC=QString::fromLocal8Bit("网格坐标");
    //不使用七参
    if(arg1 == 0)
    {
        ui->comboBox_Trans_Project->setEnabled(true);
        ui->lineEdit_Trans_MiddleLon->setEnabled(true);
        ui->lineEdit_Trans_FalseEast->setEnabled(true);
        ui->lineEdit_Trans_FalseNorth->setEnabled(true);
    }
     //使用七参
    else
    {
        ui->comboBox_Trans_Project->setEnabled(false);
        ui->lineEdit_Trans_MiddleLon->setEnabled(false);
        ui->lineEdit_Trans_FalseEast->setEnabled(false);
        ui->lineEdit_Trans_FalseNorth->setEnabled(false);
    }
}

void qTransWindow::on_menu_seven_action_caculate_triggered()
{
    SevenParameterDialog spD(this);
    spD.exec();
}

void qTransWindow::on_checkBox_Trans_UserServenParm_clicked()
{

}

void qTransWindow::on_lineEdit_Trans_MiddleLon_editingFinished()
{
    if(ui->comboBox_Trans_Project->currentText() == "UTM")//投影类型为UTM
    {
        if(ui->lineEdit_Trans_MiddleLon->text().right(1) == "S")
        {
            ui->lineEdit_Trans_FalseNorth->setText("10000000");
        }
        else
        {
            ui->lineEdit_Trans_FalseNorth->setText("0");
        }
    }

}

void qTransWindow::on_pushButton_Trans_ClearData_clicked()
{
    ui->lineEdit_Trans_Source_X->setText("");
    ui->lineEdit_Trans_Source_Y->setText("");
    ui->lineEdit_Trans_Source_Z->setText("");
    ui->lineEdit_Trans_Target_X->setText("");
    ui->lineEdit_Trans_Target_Y->setText("");
    ui->lineEdit_Trans_Target_Z->setText("");
}


void qTransWindow::on_pushButton_ChangeZone_clicked()
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

void qTransWindow::on_pushButton_ChangeZone_SelectFile_clicked()
{
    QString csvFileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择源文件"), UserFilePath, QString::fromLocal8Bit("文本文件(*.csv)"), 0);
    if (!csvFileName.isEmpty())
    {
        ui->lineEdit_ChangeZone_FilePath->setText(csvFileName);
    }
    else
    {
        return;
    }
    QFileInfo fileInfo (csvFileName);
    UserFilePath = fileInfo.filePath();
}



void qTransWindow::on_pushButton_ChangeZone_File_clicked()
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
    QFileInfo fileInfo (csvfileName);
    UserFilePath = fileInfo.filePath();
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

void qTransWindow::on_comboBox_ChangeZone_Project_activated(int index)
{

}

void qTransWindow::on_comboBox_ChangeZone_Project_activated(const QString &arg1)
{

}



void qTransWindow::on_comboBox_Trans_Project_activated(int index)
{

}

void qTransWindow::on_comboBox_Trans_Project_activated(const QString &arg1)
{

}

void qTransWindow::on_comboBox_AngleTran_Source_activated(int index)
{

}

void qTransWindow::on_comboBox_AngleTran_Source_activated(const QString &arg1)
{

}
