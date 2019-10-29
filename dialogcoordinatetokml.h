#ifndef DIALOGCOORDINATETOKML_H
#define DIALOGCOORDINATETOKML_H
#include <QDialog>
#include <QSharedPointer>
#include "SurveyMath/SurveyMath.h"
#include "SurveyMath/BasePoint.h"
#include "SurveyPoint.h"
#include "kml/dom.h"

using namespace SurveyMath;
using namespace  kmldom;
namespace Ui {
class DialogCoordinateToKml;
}

class DialogCoordinateToKml : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCoordinateToKml(QWidget *parent = nullptr);
    ~DialogCoordinateToKml();

private slots:
    void on_pushButton_Tans_TansFile_clicked();

    void on_pushButton_Tans_SelectFile_clicked();

private:
    Ui::DialogCoordinateToKml *ui;
    PlacemarkPtr CreateLineStringPlacemark(QVector<BasePoint> &points,QString name );
    PlacemarkPtr CreatePointPlacemark(BasePoint &location,QString name);

    QSharedPointer<KmlFactory> kmlFactory ;
    QVector<SurveyPoint> sourcePointVector;
    QVector<BasePoint> targetPointVector;
    QVector<BasePoint> inputPointVector;
    QVector<BasePoint> outputPointVector;
};

#endif // DIALOGCOORDINATETOKML_H
