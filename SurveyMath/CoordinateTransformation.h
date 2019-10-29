#ifndef COORDINATETRANSFORMATION_H
#define COORDINATETRANSFORMATION_H
#include "SurveyMath\BasePoint.h"
#include "SurveyMath\Projection.h"
namespace SurveyMath {
	class CoordinateTransformation
	{
	public:
        CoordinateTransformation(){}

	public:
		//转大地坐标转换为空间直角坐标
		static GeocentricCoordinate GeodeticToGeocentric(Projection prj, GeodeticCoordinate GeodeticPoint);
		//转空间直角坐标换为大地坐标
		static GeodeticCoordinate GeocentricToGeodetic(Projection prj, GeocentricCoordinate GeocentricPoint);

		//高斯坐标正算
        static CartesianCoordinate PositiveGauss(Projection prj, GeodeticCoordinate GeodeticPoint);
		static CartesianCoordinate PositiveUTM(GeodeticCoordinate GeodeticPoint);
		//高斯坐标反算
		static GeocentricCoordinate InverseGauss(Projection prj, CartesianCoordinate CartesianPoint);
		static	GeocentricCoordinate InverseUTM(int Zone, bool IsSouth, CartesianCoordinate CartesianPoint);

	private:
		//平面坐标转大地坐标
		static CartesianCoordinate GeodeticToCartesian(Projection prj, GeodeticCoordinate  GeodeticPoint);
		//大地坐标转平面坐标
		static GeocentricCoordinate CartesianToGeodetic(Projection prj, CartesianCoordinate CartesianPoint);

		static double  X0(double B, double a, double e1);
		//子午线弧长
		static double  FB0(double B, double a, double e1);
		//底点纬度用到的函数
		static double  FB1(double B, double a, double e1);
	};
}
#endif //COORDINATETRANSFORMATION_H
