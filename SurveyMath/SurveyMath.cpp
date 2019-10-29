
#include "SurveyMath\SurveyMath.h"
#include <QDebug>
namespace SurveyMath {
    /**将弧度转换为以度为单位的角度*/
    double RadianToDegree(double Radian)
    {
        double AngleDEG;
        double AngleRadian = Radian;
        AngleDEG = AngleRadian * 180.0 / PI;
        return AngleDEG;
    }
    /**将以度为单位的角度转换为弧度*/
    double DegreeToRadian(double AngleDEG)
    {
        return AngleDEG * PI / 180.0;
    }
    /**将弧度转换为以度度分秒*/
    double RadianToDMS(double Radian)
    {
        //1,将弧度转换为度
        double dDeg = RadianToDegree(Radian);
        //2,将度转换为度分秒
        return DegreeToDMS(dDeg);
    }
    /**将度分秒角度转换为弧度*/
    double DMSToRadian(double AngleDEG)
    {
        //1,将度分秒转换为度
        double dR = DMSToDegree(AngleDEG);
        //2,将度转换为弧度
        return DegreeToRadian(dR);
    }
    //将度分秒转换为十进制度
    double DMSToDegree(double DMS)
    {
        double AngleDEG, AngleDMS;
        double Degree, Minute, Second;
        double tSymbol = 1;
        if (DMS < 0)
        {
            tSymbol = -1;
        }
        AngleDMS = abs(DMS);
        Degree = int(AngleDMS);
        Minute = int(((AngleDMS - Degree) * 10000 + 0.5) / 100);
        Second = ((AngleDMS - Degree) * 100.0 - Minute) * 100.0;
        AngleDEG = Degree + Minute / 60.0 + Second / 3600.0;
        return tSymbol * AngleDEG;
    }
    //将十进制度转换为度分秒
    double DegreeToDMS(double DEG)
    {
        double AngleDMS, AngleDEG;
        double Degree, Minute, Second;
        double tSymbol = 1;
        if (DEG < 0)
        {
            tSymbol = -1;
        }
        AngleDEG = abs(DEG);
        Degree = int((AngleDEG*1000000000 + 5)/1000000000);
        Minute = int((AngleDEG - Degree) * 60);
        Second = ((AngleDEG - Degree) * 60.0 - Minute) * 60;
        AngleDMS = Degree + Minute / 100.0 + Second / 10000.0;
        return tSymbol * AngleDMS;
    }

    /**将以度为单位的角度转换为百分度*/
    double DegreeToGradian(double AngleDEG)
    {
        double dT = 400.0 / 360.0;
        return AngleDEG * dT;
    }
    /**将百分度换为以度为单位的角度*/
    double GradianToDegree(double Gradian)
    {
        double dT = 400.0 / 360.0;
        return Gradian / dT;
    }

}

