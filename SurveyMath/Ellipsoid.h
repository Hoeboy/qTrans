#ifndef ELLIPSOID_H
#define ELLIPSOID_H
#include "SurveyMath\SurveyMath.h"
namespace SurveyMath {
	class Ellipsoid
	{
	public:
		Ellipsoid()
		{
			LongAxis = 6378137;
			Flattening = 1.0/298.257223563;
		}
		Ellipsoid(double a, double f)
		{
			LongAxis = a;
			Flattening = f;
		}
	private:
		double LongAxis;//椭球长半轴
		double Flattening;//椭球扁率
	public:
		double inline GetLongAxis()//椭球长半轴
		{
			return LongAxis;
		}
		double inline GetFlattening()//椭球扁率
		{
			return Flattening;
		}
		double inline GetFirstEccentricity()//第一偏心率e
		{
			return sqrt(2.0 * Flattening - Flattening * Flattening);
		}
		double inline GetSecondEccentricity()//第二偏心率e'
		{
			double b = LongAxis - LongAxis * Flattening;
			return sqrt(LongAxis * LongAxis - b * b) / b;
		}

	};
}
#endif //ELLIPSOID_H
