
#include "dialogcoordinatetokml.h"
#include "ui_dialogcoordinatetokml.h"
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>


DialogCoordinateToKml::DialogCoordinateToKml(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCoordinateToKml)
{
    ui->setupUi(this);
    kmlFactory =QSharedPointer<KmlFactory>(KmlFactory::GetFactory());
}

DialogCoordinateToKml::~DialogCoordinateToKml()
{
    delete ui;
}
PlacemarkPtr DialogCoordinateToKml::CreatePointPlacemark(BasePoint &location,QString name)
{
  KmlFactory* factory = KmlFactory::GetFactory();
  CoordinatesPtr coordinates = factory->CreateCoordinates();
  coordinates->add_latlngalt(location.x, location.y,location.z);
  PointPtr point = factory->CreatePoint();
  point->set_coordinates(coordinates);
  PlacemarkPtr placemark = factory->CreatePlacemark();

  placemark->set_name(name.toStdString());
  placemark->set_geometry(point);
  placemark->set_styleurl("http://maps.google.com/mapfiles/kml/shapes/placemark_circle.png");
  return placemark;
}
PlacemarkPtr DialogCoordinateToKml::CreateLineStringPlacemark(QVector<BasePoint> &points,QString name)
{
  KmlFactory* factory = KmlFactory::GetFactory();

  CoordinatesPtr coordinates = factory->CreateCoordinates();
  for(BasePoint bP:points)
  {
      coordinates->add_latlngalt(bP.x, bP.y, bP.z);
  }

  LineStringPtr linestring = factory->CreateLineString();
  linestring->set_extrude(true);
  linestring->set_tessellate(true);
 // linestring->set_altitudemode(kmldom::ALTITUDEMODE_ABSOLUTE);
  linestring->set_coordinates(coordinates);

  PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_geometry(linestring);
  placemark->set_name(name.toStdString());
  return placemark;
}
void DialogCoordinateToKml::on_pushButton_Tans_TansFile_clicked()
{    
    sourcePointVector.clear();
    targetPointVector.clear();
    //读取原始点到数组
    if (! ui->lineEdit_Trans_FilePath->text().isEmpty())
    {
       QFile sourceFile(ui->lineEdit_Trans_FilePath->text());
       if(!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
       {
          return;
       }

       QTextStream inSourceFile(&sourceFile);

       //读取源坐标
       while(!inSourceFile.atEnd())
       {
           QStringList lineList;
           QString strLine = inSourceFile.readLine();
           lineList = strLine.split(",", QString::SkipEmptyParts);
           if(lineList.size()<3)
           {
               return;
           }
           SurveyPoint sP;
           sP.PointName = lineList.at(0);
           sP.x=lineList.at(1).toDouble();
           sP.y=lineList.at(2).toDouble();
           if(lineList.size()<4)
           {
                sP.z=0;
           }
           else
           {
                sP.z=lineList.at(3).toDouble();
           }

           if(lineList.size()<5)
           {
               sP.Code = "0";
           }
           else
           {
               sP.Code = lineList.at(4);
           }

           sourcePointVector.push_back(sP);
       }
    }
    else
    {
       return;
    }


   // FolderPtr folder = kmlFactory->CreateFolder();
    DocumentPtr document = kmlFactory->CreateDocument();

    //点样式普通模式
    StylePtr pointstylenormal = kmlFactory->CreateStyle();
    pointstylenormal->set_id("pointstylenormal");
    IconStylePtr iconstyle = kmlFactory->CreateIconStyle();
    iconstyle->set_scale(1.1);
    IconStyleIconPtr iconNormal=kmlFactory->CreateIconStyleIcon();
    iconNormal->set_href("http://maps.google.com/mapfiles/kml/shapes/placemark_circle.png");
    iconstyle->set_icon(iconNormal);
    pointstylenormal->set_iconstyle(iconstyle);
    document->add_styleselector(pointstylenormal);
    //点样式选择模式
    StylePtr pointstylehighlight = kmlFactory->CreateStyle();
    pointstylehighlight->set_id("pointstylehighlight");
    iconstyle = kmlFactory->CreateIconStyle();
    iconstyle->set_scale(1.4);
    IconStyleIconPtr iconHighlight=kmlFactory->CreateIconStyleIcon();
    iconHighlight->set_href("http://maps.google.com/mapfiles/kml/shapes/placemark_circle.png");
    iconstyle->set_icon(iconHighlight);
    pointstylehighlight->set_iconstyle(iconstyle);
    document->add_styleselector(pointstylehighlight);
  
    //创建映射
    StyleMapPtr pointstylemap = kmlFactory->CreateStyleMap();
    pointstylemap->set_id("pointstylemap");
    PairPtr pair = kmlFactory->CreatePair();
    pair->set_key(kmldom::STYLESTATE_NORMAL);
    pair->set_styleurl("#pointstylenormal");
    pointstylemap->add_pair(pair);
  
    pair = kmlFactory->CreatePair();
    pair->set_key(kmldom::STYLESTATE_HIGHLIGHT);
    pair->set_styleurl("#pointstylehighlight");
    pointstylemap->add_pair(pair);
    document->add_styleselector(pointstylemap);

    //线样式普通模式
    LineStylePtr linenormalStyle= kmlFactory->CreateLineStyle();
    kmlbase::Color32 color32;
    color32.set_color_abgr(255,255,0,0);
    linenormalStyle->set_color(color32);
    PolyStylePtr  polyStyle=kmlFactory->CreatePolyStyle();
    polyStyle->set_fill(0);
    StylePtr linestringnormalStyle = kmlFactory->CreateStyle();
    linestringnormalStyle->set_id("linestringnormalstyle");
    linestringnormalStyle->set_linestyle(linenormalStyle);
    linestringnormalStyle->set_polystyle(polyStyle);
    document->add_styleselector(linestringnormalStyle);

    //线样式选中模式
    LineStylePtr linehighlightStyle= kmlFactory->CreateLineStyle();
    color32.set_color_abgr(255,0,0,255);
    linehighlightStyle->set_color(color32);
    polyStyle=kmlFactory->CreatePolyStyle();
    polyStyle->set_fill(0);
    StylePtr linestringhighlightStyle = kmlFactory->CreateStyle();
    linestringhighlightStyle->set_id("linestringhighlightstyle");
    linestringhighlightStyle->set_linestyle(linehighlightStyle);
    linestringhighlightStyle->set_polystyle(polyStyle);
    document->add_styleselector(linestringhighlightStyle);

    StyleMapPtr linestylemap = kmlFactory->CreateStyleMap();
    linestylemap->set_id("linestylemap");
    PairPtr linenormalpair = kmlFactory->CreatePair();
    linenormalpair->set_key(kmldom::STYLESTATE_NORMAL);
    linenormalpair->set_styleurl("#linestringnormalstyle");
    linestylemap->add_pair(linenormalpair);
    linenormalpair = kmlFactory->CreatePair();
    linenormalpair->set_key(kmldom::STYLESTATE_HIGHLIGHT);
    linenormalpair->set_styleurl("#linestringhighlightstyle");
    linestylemap->add_pair(linenormalpair);
    document->add_styleselector(linestylemap);

    //为所有点创建标记，并转换DMS到DEG
    for(SurveyPoint bP:sourcePointVector)
    {
        BasePoint tP;
        tP.x=DMSToDegree(bP.x);
        tP.y=DMSToDegree(bP.y);
        tP.z=bP.z;
        PlacemarkPtr placemark =CreatePointPlacemark(tP,bP.PointName);
        placemark->set_styleurl("#pointstylemap");
        document->add_feature(placemark);
        targetPointVector.push_back(tP);
    }
    if(ui->radioButtonLine->isChecked())
    {
        PlacemarkPtr placemark = CreateLineStringPlacemark(targetPointVector,"Path");
        placemark->set_styleurl("#linestylemap");
        document->add_feature(placemark);
    }
    //识别编码
    if(ui->radioButtonCode->isChecked())
    {
        QString currentCode = "0";
        QVector<BasePoint> currentPointVector;
        currentPointVector.clear();
        for(SurveyPoint bP:sourcePointVector)
        {
            BasePoint tP;
            tP.x=DMSToDegree(bP.x);
            tP.y=DMSToDegree(bP.y);
            tP.z=bP.z;

            if(bP.Code != "0" && bP.Code !=currentCode)
            {
                if(currentPointVector.size()>1)
                {
                    PlacemarkPtr placemark = CreateLineStringPlacemark(currentPointVector,currentCode);
                    placemark->set_styleurl("#linestylemap");
                    document->add_feature(placemark);
                }
                currentPointVector.clear();
                currentPointVector.push_back(tP);
                currentCode = bP.Code;
            }
            if(bP.Code != "0" && bP.Code ==currentCode)
            {
                currentPointVector.push_back(tP);
            }
        }
    }
    KmlPtr kml = kmlFactory->CreateKml();
    kml->set_feature(document);
    QString csvfileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("请输入文件名"),QDir::currentPath(), QString::fromLocal8Bit("KML文件(*.kml)"),0);
    QFileInfo fileInfo (csvfileName);
    QFile csvFile(csvfileName);
    if(!csvFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return ;
    }

    QTextStream txtOutput(&csvFile);
    txtOutput<<QString::fromStdString( kmldom::SerializePretty(kml));
    csvFile.close();;
}

void DialogCoordinateToKml::on_pushButton_Tans_SelectFile_clicked()
{
    QString csvFileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择目标文件"), "/", QString::fromLocal8Bit("文本文件(*.csv)"), 0);
    if (!csvFileName.isEmpty())
    {
       ui->lineEdit_Trans_FilePath->setText(csvFileName);
    }
    else
    {
       return;
    }
}
