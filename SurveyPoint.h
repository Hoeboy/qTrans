#ifndef SURVEYPOINT_H
#define SURVEYPOINT_H
#include <QString>
#include "SurveyMath/BasePoint.h"
namespace SurveyMath {
    class SurveyPoint:public BasePoint
    {
    public:
        QString PointName;
        QString Code;
    };

}
#endif  //SURVEYPOINT_H
