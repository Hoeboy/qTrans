#ifndef SEVENPARAMETERHASSISTANT_H
#define SEVENPARAMETERHASSISTANT_H
#include <QString>
#include "SurveyMath/SevenParameter.h"
#include "SurveyMath/Projection.h"
using namespace SurveyMath;
class SevenParameterhAssistant
{
public:
    SevenParameterhAssistant();
    static bool isSet;
    static int Type;
    static Projection sourcePrjoject;
    static Projection targetPrjoject;
    static SevenParameter sevenParameter;
    static QString projectName;
};

#endif // SEVENPARAMETERHASSISTANT_H
