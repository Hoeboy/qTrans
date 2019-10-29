#ifndef BASEPOINT_H
#define BASEPOINT_H
namespace SurveyMath {
	class BasePoint
	{
	public:
		BasePoint()
		{
			x = 0;
			y = 0;
			z = 0;
		}
		BasePoint(double _x, double _y, double _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}
	public:
		double x;
		double y;
		double z;
	};
	typedef BasePoint CartesianCoordinate;
	typedef BasePoint GeodeticCoordinate;
	typedef BasePoint GeocentricCoordinate;
}
#endif  //BASEPOINT_H
