#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include "ui_dialogchangeangle.h"
#include "dialogchangeangle.h"
DialogChangeAngle::DialogChangeAngle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChangeAngle)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不可编辑
}

DialogChangeAngle::~DialogChangeAngle()
{
    delete ui;
}
double DialogChangeAngle::ChangeAngle(double inputAngle)
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

void DialogChangeAngle::on_pushButton_AngleTran_Point_clicked()
{
    double dSource=ui->lineEdit_AngleTran_Soruce->text().toDouble();
    double dTarget = ChangeAngle(dSource);
    ui->lineEdit_AngleTran_Target->setText(QString::number(dTarget,14,9));
}

void DialogChangeAngle::on_comboBox_AngleTran_Source_currentIndexChanged(const QString &arg1)
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

void DialogChangeAngle::on_pushButton_Another_AngleTran_SelectFile_clicked()
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

    QFile sourceFile(ui->lineEdit_AngleTran_FileName->text());
    QTextStream inSourceFile(&sourceFile);

    if(!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       return;
    }
    //读取源坐标
    QStringList lineList;
    QString strLine = inSourceFile.readLine();
    sourceFile.close();
    lineList = strLine.split(",", QString::SkipEmptyParts);

    QStringList headerLabels;
    for (int i=1;i<=lineList.count();i++)
    {

        headerLabels.append(QString::fromLocal8Bit("列")+QString::number(i));
    }

    ui->tableWidget->setColumnCount(lineList.size());
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->verticalHeader()->setVisible(false); // 隐藏水平header
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);

    for(int i = 0; i < ui->tableWidget->columnCount(); i++)
    {
       QTableWidgetItem   *item = new QTableWidgetItem(lineList[i]);
       item->setTextAlignment(Qt::AlignCenter);
       ui->tableWidget->setItem(0, i,item);
    }
}

void DialogChangeAngle::on_pushButton_AngleTran_File_clicked()
{

    std::vector<int> vecItemIndex;//保存选中行的索引
    QItemSelectionModel *selections =  ui->tableWidget->selectionModel(); //返回当前的选择模式
    QModelIndexList selectedsList = selections->selectedIndexes(); //返回所有选定的模型项目索引列表

    for (int i = 0; i < selectedsList.count(); i++)
    {
        vecItemIndex.push_back(selectedsList.at(i).column());
    }
    std::sort(vecItemIndex.begin(), vecItemIndex.end());
    vecItemIndex.erase(std::unique(vecItemIndex.begin(), vecItemIndex.end()), vecItemIndex.end());

    if(vecItemIndex.size()<1)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("请选择需要转换的列!"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }



    QFile sourceFile(ui->lineEdit_AngleTran_FileName->text());
    QTextStream inSourceFile(&sourceFile);
    QVector<SurveyPoint> inputPointVector;
    if(!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       return;
    }
    QString csvfileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("请输入文件名"), UserFilePath, QString::fromLocal8Bit("csv文件(*.csv)"),0);
    QFileInfo fileInfo (csvfileName);
    UserFilePath = fileInfo.filePath().left( fileInfo.filePath().size() -4) + "T.CSV" ;
    QFile csvFile(csvfileName);
    if(!csvFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return ;
    }

    QTextStream txtOutput(&csvFile);
    //读取源坐标
    while(!inSourceFile.atEnd())
    {


        QStringList lineList;
        QString strLine = inSourceFile.readLine();
        lineList = strLine.split(",", QString::SkipEmptyParts);
        int beginColumn = vecItemIndex[0];
        int endColumn = vecItemIndex[vecItemIndex.size()-1];
        for(int i=0;i<lineList.size();i++)
        {
            if(i<beginColumn || i>endColumn)
            {
                txtOutput<<lineList[i];
            }
            else
            {
                qDebug()<<i;
                txtOutput<<qSetRealNumberPrecision(11)<<fixed
                 << ChangeAngle(lineList[i].toDouble());
            }
            if(i<lineList.size() - 1)
            {
               txtOutput<<",";
            }
        }
            txtOutput<<endl;
    }
    sourceFile.close();
    csvFile.close();
}
