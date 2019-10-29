#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include "sevenparameterdialog.h"
#include "ui_sevenparameterdialog.h"
#include "sevenparameterhassistant.h"
SevenParameterDialog::SevenParameterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SevenParameterDialog)
{
    ui->setupUi(this);
    Ellipsoid ell_wgs84(6378137.0, 1/298.257223563);
    Ellipsoid ell_bj54(6378245.0, 1/298.3);
    Ellipsoid ell_xian80(6378140.0, 1/298.257);
    Ellipsoid ell_cscg2000(6378137.0, 1/298.257222101);
    prj_bj54.SetDatum(ell_bj54);
    prj_wgs84.SetDatum(ell_wgs84);
    prj_xian80.SetDatum(ell_xian80);
    prj_cscg2000.SetDatum(ell_cscg2000);

    ProjectName[0]=QString::fromLocal8Bit("WGS84");
    ProjectName[1]=QString::fromLocal8Bit("国家2000");
    ProjectName[2]=QString::fromLocal8Bit("北京54");
    ProjectName[3]=QString::fromLocal8Bit("西安80");


    CoordinateType[0]=QString::fromLocal8Bit("大地坐标");
    CoordinateType[1]=QString::fromLocal8Bit("网格坐标");

    isCaculate=false;
    pRevLongitude = new  QRegExpValidator(QRegExp("^-?(180|1?[0-7]?\\d(\\.\\d{1,10})?)$")) ;//正则表达式经度匹配
    ui->lineEdit_MiddleLon->setValidator(pRevLongitude);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不可编辑
}

SevenParameterDialog::~SevenParameterDialog()
{
    delete ui;
    delete pRevLongitude;
}


void SevenParameterDialog::on_pushButton_Caculate_clicked()
{

    if(ui->tableWidget->rowCount() < 3)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("计算七参数不能少于三个点!"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    ui->pushButton_Enter->setEnabled(true);
    ui->pushButton_View->setEnabled(true);

    inputPointVector.clear();
    outputPointVector.clear();

    //源坐标系统
    sourcePrjoject = prj_wgs84;

     //目标坐标系统
    if(ui->comboBox_Project->currentIndex()==0)//国家2000
    {
        targetPrjoject = prj_cscg2000;
    }
    if(ui->comboBox_Project->currentIndex()==1)//北京54
    {
        targetPrjoject = prj_bj54;
    }
    if(ui->comboBox_Project->currentIndex()==2)//西安80
    {
        targetPrjoject = prj_xian80;
    }

    //一步法
    if(ui->comboBox_CaculateMethod->currentIndex() == 1)
    {
        double dMiddle = 0;
        int iCount = sourcePointVector.size();
        for(int i=0;i<iCount;i++)
        {
            dMiddle = dMiddle + DMSToRadian(sourcePointVector[i].y);
        }
        dMiddle = dMiddle / iCount;//计算中央子午线
        sourcePrjoject.SetFalseEasting(500000);
        sourcePrjoject.SetFalseNorthing(0);
        sourcePrjoject.SetCentralMeridian(dMiddle);
        targetPrjoject = sourcePrjoject;//一步法时设定源坐标系统和目标坐标系统都为WGS84
        //一步法时将WGS84大地坐标转换成WGS84平面坐标,然后利用WGS84平面坐标和目标坐标平面坐标求转换参数
        for(int i=0;i<iCount;i++)
        {
            //将度分秒转换为弧度
            BasePoint gP(DMSToRadian(sourcePointVector[i].x),DMSToRadian(sourcePointVector[i].y),sourcePointVector[i].z);
           //将WGS84大地坐标转换成WGS84平面坐标
            BasePoint tP = CoordinateTransformation::PositiveGauss(sourcePrjoject,gP);

            inputPointVector.push_back(tP);
        }
        //目标坐标为平面坐标
        outputPointVector = targetPointVector;


    }
    if(ui->comboBox_CaculateMethod->currentIndex() == 0)//经典七参
    {
        double  middleLongitude=DMSToRadian( ui->lineEdit_MiddleLon->text().toDouble());
        sourcePrjoject.SetCentralMeridian(middleLongitude);
        targetPrjoject.SetCentralMeridian(middleLongitude);

        //读取WGS84大地坐标
        for(int i=0;i<sourcePointVector.size();i++)
        {
            //将度分秒转换为弧度
            BasePoint gP(DMSToRadian(sourcePointVector[i].x),DMSToRadian(sourcePointVector[i].y),sourcePointVector[i].z);
           //将弧度单位的大地坐标转换成地心直角坐标
            BasePoint tP = CoordinateTransformation::GeodeticToGeocentric(sourcePrjoject,gP);
            inputPointVector.push_back(tP);
        }

        //目标坐标平面坐标
        for(int i=0;i<targetPointVector.size();i++)
        {
           //将平面坐标转换成大地坐标
            BasePoint gP = CoordinateTransformation::InverseGauss(targetPrjoject,targetPointVector[i]);
           //将弧度单位的大地坐标转换成地心直角坐标
            BasePoint tP = CoordinateTransformation::GeodeticToGeocentric(targetPrjoject,gP);
            outputPointVector.push_back(tP);
        }
     }

    int iCount = inputPointVector.size();
    sevenParameter.Calculate(inputPointVector.data(),outputPointVector.data(),iCount);//计算七参数
    iCount = 0;
    //读取源坐标地心直角坐标
    for(BasePoint sP:inputPointVector)
    {


        //通过转换参数将WGS84地心直角坐标转换成目标坐标系地心直角坐标(经典七参时），平面坐标转换成目标坐标平面坐标（一步法时）
       BasePoint cP = sevenParameter.Transform(sP);
       BasePoint tP;
       if(ui->comboBox_CaculateMethod->currentIndex() == 0)//经典七参
       {
           //将目标地心直角坐标系转换成大地坐标
           BasePoint gP = CoordinateTransformation::GeocentricToGeodetic(targetPrjoject,cP);
           //将目标大地坐标系转换成高斯直角坐标
           tP = CoordinateTransformation::PositiveGauss(targetPrjoject,gP);
       }
       else//一步法
       {
            tP = cP;
       }

       QTableWidgetItem *tItemX = new QTableWidgetItem(QString::number(targetPointVector[iCount].x - tP.x,'f',3));
       tItemX->setTextAlignment(Qt::AlignCenter);
       QTableWidgetItem *tItemY = new QTableWidgetItem(QString::number(targetPointVector[iCount].y - tP.y,'f',3));
       tItemY->setTextAlignment(Qt::AlignCenter);
       QTableWidgetItem *tItemZ = new QTableWidgetItem(QString::number(targetPointVector[iCount].z - tP.z,'f',3));
       tItemZ->setTextAlignment(Qt::AlignCenter);
       ui->tableWidget->setItem(iCount, 7,tItemX);
       ui->tableWidget->setItem(iCount, 8,tItemY);
       ui->tableWidget->setItem(iCount, 9,tItemZ);
       iCount++;
    }


    QMessageBox::about(this, QString::fromLocal8Bit("计算结果"),
                       QString::fromLocal8Bit("DX(米) = ") + QString::number(sevenParameter.DX,10,10)
                       + QString::fromLocal8Bit("\r\nDY(米) = " )+ QString::number(sevenParameter.DY,10,10)
                       + QString::fromLocal8Bit("\r\nDZ(米) = ") + QString::number(sevenParameter.DZ,10,10)
                       + QString::fromLocal8Bit("\r\nRX(秒) = ") + QString::number(sevenParameter.GetSecondRX(),10,10)
                       + QString::fromLocal8Bit("\r\nRY(秒) = ") + QString::number(sevenParameter.GetSecondRY(),10,10)
                       + QString::fromLocal8Bit("\r\nRZ(秒) = ") + QString::number(sevenParameter.GetSecondRZ(),10,10)
                       + QString::fromLocal8Bit("\r\nPPM = ") + QString::number(sevenParameter.GetPPM(),10,10));
}

void SevenParameterDialog::on_pushButton_Close_clicked()
{
    close();
}


void SevenParameterDialog::on_pushButton_Target_File_clicked()
{
    QString csvFileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择目标文件"), ".", QString::fromLocal8Bit("文本文件(*.csv)"), 0);
    if (!csvFileName.isEmpty())
    {
        ui->lineEdit_GCP_FilePath->setText(csvFileName);

        QFile sourceFile(ui->lineEdit_GCP_FilePath->text());
        if(!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
           return;
        }

        QTextStream inSourceFile(&sourceFile);


        ui->tableWidget->verticalHeader()->setVisible(false); // 隐藏水平header
        //读取源坐标
        while(!inSourceFile.atEnd())
        {
            QStringList lineList;
            QString strLine = inSourceFile.readLine();
            lineList = strLine.split(",", QString::SkipEmptyParts);
            BasePoint sP;
            sP.x=lineList.at(1).toDouble();
            sP.y=lineList.at(2).toDouble();
            sP.z=lineList.at(3).toDouble();
            sourcePointVector.push_back(sP);
            BasePoint tP;
            tP.x=lineList.at(4).toDouble();
            tP.y=lineList.at(5).toDouble();
            tP.z=lineList.at(6).toDouble();
            targetPointVector.push_back(tP);
            int rowIndex =  ui->tableWidget->rowCount();
             ui->tableWidget->setRowCount(rowIndex + 1);//总行数增加1

            for(int i = 0; i <  ui->tableWidget->columnCount(); i++)
            {

               if(i<7)
               {
                   QTableWidgetItem *tItem=new QTableWidgetItem(lineList[i]);
                   tItem->setTextAlignment(Qt::AlignCenter);
                   ui->tableWidget->setItem(rowIndex, i,tItem);
               }
               else
               {
                   QTableWidgetItem *tItem=new QTableWidgetItem(QString::fromLocal8Bit("未计算"));
                   tItem->setTextAlignment(Qt::AlignCenter);
                   ui->tableWidget->setItem(rowIndex, i,tItem);
               }
               // ui->tableWidget->item(rowIndex, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            }

        }
    }
    else
    {
        return;
    }
}


void SevenParameterDialog::on_pushButton_View_clicked()
{
    QMessageBox::about(this, QString::fromLocal8Bit("查看7参数"),
                       QString::fromLocal8Bit("DX(米) = ") + QString::number(sevenParameter.DX,10,10)
                       + QString::fromLocal8Bit("\r\nDY(米) = " )+ QString::number(sevenParameter.DY,10,10)
                       + QString::fromLocal8Bit("\r\nDZ(米) = ") + QString::number(sevenParameter.DZ,10,10)
                       + QString::fromLocal8Bit("\r\nRX(秒) = ") + QString::number(sevenParameter.GetSecondRX(),10,10)
                       + QString::fromLocal8Bit("\r\nRY(秒) = ") + QString::number(sevenParameter.GetSecondRY(),10,10)
                       + QString::fromLocal8Bit("\r\nRZ(秒) = ") + QString::number(sevenParameter.GetSecondRZ(),10,10)
                       + QString::fromLocal8Bit("\r\nPPM = ") + QString::number(sevenParameter.GetPPM(),10,10));

}

void SevenParameterDialog::on_pushButton_Enter_clicked()
{
    //一步法
    if(ui->comboBox_CaculateMethod->currentIndex() == 1)
    {
        SevenParameterhAssistant::Type = 0;
        SevenParameterhAssistant::projectName = QString::fromLocal8Bit("一");
    }
    else
    {
        SevenParameterhAssistant::Type = 1;
        SevenParameterhAssistant::projectName = ui->comboBox_Project->currentText();
    }
    SevenParameterhAssistant::sourcePrjoject = sourcePrjoject;
    SevenParameterhAssistant::targetPrjoject = targetPrjoject;
    SevenParameterhAssistant::sevenParameter = sevenParameter;
    accept();
}

void SevenParameterDialog::on_pushButton_Cancel_clicked()
{
    reject();
}

void SevenParameterDialog::on_comboBox_CaculateMethod_activated(const QString &arg1)
{
    if(arg1==QString::fromLocal8Bit("经典七参"))
    {
        ui->comboBox_Project->setEnabled(true);
        ui->lineEdit_MiddleLon->setEnabled(true);
        ui->lineEditt_FalseEast->setEnabled(true);
        ui->lineEdit_FalseNorth->setEnabled(true);
    }
    else
    {
        ui->comboBox_Project->setEnabled(false);
        ui->lineEdit_MiddleLon->setEnabled(false);
        ui->lineEditt_FalseEast->setEnabled(false);
        ui->lineEdit_FalseNorth->setEnabled(false);
    }

}

