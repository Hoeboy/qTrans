#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include "viewsevenparameterdialog.h"
#include "sevenparameterhassistant.h"
ViewSevenParameterDialog::ViewSevenParameterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewSevenParameterDialog)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不可编辑
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setVisible(false); // 隐藏水平header

    itemType=new QTableWidgetItem();
    itemType->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(0, 0,itemType);
    itemProject=new QTableWidgetItem();
    itemProject->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(1, 0,itemProject);
    itemMiddleLine=new QTableWidgetItem();
    itemMiddleLine->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(2, 0,itemMiddleLine);
    itemFalseEast=new QTableWidgetItem();
    itemFalseEast->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(3, 0,itemFalseEast);
    itemFalseNorth=new QTableWidgetItem();
    itemFalseNorth->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(4, 0,itemFalseNorth);

    itemDX=new QTableWidgetItem();
    itemDX->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(5, 0,itemDX);
   itemDY=new QTableWidgetItem();
    itemDY->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(6, 0,itemDY);
    itemDZ=new QTableWidgetItem();
    itemDZ->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(7, 0,itemDZ);
    itemRX=new QTableWidgetItem();
    itemRX->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(8, 0,itemRX);
    itemRY=new QTableWidgetItem();
    itemRY->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(9, 0,itemRY);
    itemRZ=new QTableWidgetItem();
    itemRZ->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(10, 0,itemRZ);
    itemPPM=new QTableWidgetItem();
    itemPPM->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(11, 0,itemPPM);

    if(SevenParameterhAssistant::isSet == true)
    {
        itemProject->setText(SevenParameterhAssistant::projectName);
        itemType->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(0, 0,itemType);

        if(SevenParameterhAssistant::Type == 0)
        {
            itemType->setText(QString::fromLocal8Bit("一步法"));
            itemMiddleLine->setText(QString::fromLocal8Bit("一"));
            itemFalseEast->setText(QString::fromLocal8Bit("一"));
            itemFalseNorth->setText(QString::fromLocal8Bit("一"));
        }
        else
        {
            itemType->setText(QString::fromLocal8Bit("经典七参"));
            itemMiddleLine->setText(QString::number(RadianToDMS(SevenParameterhAssistant::targetPrjoject.GetCentralMeridian()),'f',9));
            itemFalseEast->setText(QString::number(SevenParameterhAssistant::targetPrjoject.GetFalseEasting(),'f',9));
            itemFalseNorth->setText(QString::number(SevenParameterhAssistant::targetPrjoject.GetFalseNorthing(),'f',9));
        }
        itemDX->setText(QString::number(SevenParameterhAssistant::sevenParameter.DX,'f',9));
        itemDY->setText(QString::number(SevenParameterhAssistant::sevenParameter.DY,'f',9));
        itemDZ->setText(QString::number(SevenParameterhAssistant::sevenParameter.DZ,'f',9));
        itemRX->setText(QString::number(SevenParameterhAssistant::sevenParameter.GetSecondRX(),'f',9));
        itemRY->setText(QString::number(SevenParameterhAssistant::sevenParameter.GetSecondRY(),'f',9));
        itemRZ->setText(QString::number(SevenParameterhAssistant::sevenParameter.GetSecondRZ(),'f',9));
        itemPPM->setText(QString::number(SevenParameterhAssistant::sevenParameter.GetPPM(),'f',9));


    }
}

ViewSevenParameterDialog::~ViewSevenParameterDialog()
{
    delete ui;
}


void ViewSevenParameterDialog::on_pushButton_Caculate_clicked()
{
   SevenParameterDialog sevenParameterDialog(this);
   int iResult = sevenParameterDialog.exec();
   sevenParameterDialog.setFixedSize(340,300);
   if(iResult == 1)
   {
       itemProject->setText(SevenParameterhAssistant::projectName);
       itemType->setTextAlignment(Qt::AlignCenter);
       ui->tableWidget->setItem(0, 0,itemType);

       if(SevenParameterhAssistant::Type == 0)
       {
           itemType->setText(QString::fromLocal8Bit("一步法"));
           itemMiddleLine->setText(QString::fromLocal8Bit("一"));
           itemFalseEast->setText(QString::fromLocal8Bit("一"));
           itemFalseNorth->setText(QString::fromLocal8Bit("一"));
       }
       else
       {
           itemType->setText(QString::fromLocal8Bit("经典七参"));
           itemMiddleLine->setText(QString::number(RadianToDMS(SevenParameterhAssistant::targetPrjoject.GetCentralMeridian()),'f',9));
           itemFalseEast->setText(QString::number(SevenParameterhAssistant::targetPrjoject.GetFalseEasting(),'f',9));
           itemFalseNorth->setText(QString::number(SevenParameterhAssistant::targetPrjoject.GetFalseNorthing(),'f',9));
       }
       itemDX->setText(QString::number(SevenParameterhAssistant::sevenParameter.DX,'f',9));
       itemDY->setText(QString::number(SevenParameterhAssistant::sevenParameter.DY,'f',9));
       itemDZ->setText(QString::number(SevenParameterhAssistant::sevenParameter.DZ,'f',9));
       itemRX->setText(QString::number(SevenParameterhAssistant::sevenParameter.GetSecondRX(),'f',9));
       itemRY->setText(QString::number(SevenParameterhAssistant::sevenParameter.GetSecondRY(),'f',9));
       itemRZ->setText(QString::number(SevenParameterhAssistant::sevenParameter.GetSecondRZ(),'f',9));
       itemPPM->setText(QString::number(SevenParameterhAssistant::sevenParameter.GetPPM(),'f',9));
   }

}

void ViewSevenParameterDialog::on_pushButton_Close_clicked()
{
    close();
}







void ViewSevenParameterDialog::on_pushButton_Save_clicked()
{

}

void ViewSevenParameterDialog::on_pushButton_Input_clicked()
{

}

void ViewSevenParameterDialog::on_pushButton_Apply_clicked()
{
    for(int i=0;i<7;i++)
    {
         if(ui->tableWidget->item(i,0)->text().size() == 0)
         {
             QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("七参数不能为空!"), QMessageBox::Ok, QMessageBox::Ok);
             return;
         }
    }
    SevenParameterhAssistant::isSet = true;
    accept();
}
