#ifndef PROJECTION_H
#define PROJECTION_H
#include "SurveyMath\Ellipsoid.h"
#include "SurveyMath\Enumerate.h"
namespace SurveyMath {
	class Projection
	{
	public:
        Projection()
        {
            CentralMeridianScaleFactor = 1.0;
            CentralMeridian =DegreeToRadian(117.0);
            OriginLatitude = 0.0;
            FalseEasting = 500000.0;
            FalseNorthing = 0.0;
        }
		Projection(Ellipsoid datum, double centralMeridianScaleFactor, double centralMeridian, double originLatitude, double falseEasting, double falseNorthing)
		{
			Datum = datum;
			CentralMeridianScaleFactor = centralMeridianScaleFactor;
			CentralMeridian = centralMeridian;
			OriginLatitude = originLatitude;
			FalseEasting = falseEasting;
			FalseNorthing = falseNorthing;
		}

	protected:
		Ellipsoid Datum;
		double CentralMeridianScaleFactor;
		double CentralMeridian;
		double OriginLatitude;
		double FalseEasting;
		double FalseNorthing;
	public:

		void inline SetDatum(Ellipsoid datum)
		{
			Datum = datum;
		}
		void inline SetCentralMeridianScaleFactor(double centralMeridianScaleFactor)
		{
			CentralMeridianScaleFactor = centralMeridianScaleFactor;
		}
		void inline SetCentralMeridian(double centralMeridian)
		{
			CentralMeridian = centralMeridian;
		}
		void inline SetOriginLatitude(double originLatitude)
		{
			OriginLatitude = originLatitude;
		}
		void inline SetFalseEasting(double falseEasting)
		{
			FalseEasting = falseEasting;
		}
		void inline SetFalseNorthing(double falseNorthing)
		{
			FalseNorthing = falseNorthing;
		}
		Ellipsoid inline GetDatum()
		{
			return Datum;
		}

		double inline GetCentralMeridianScaleFactor()
		{
			return CentralMeridianScaleFactor;
		}
		double inline GetCentralMeridian()
		{
			return CentralMeridian;
		}
		double inline GetOriginLatitude()
		{
			return OriginLatitude;
		}
		double inline GetFalseEasting()
		{
			return FalseEasting;
		}
		double inline GetFalseNorthing()
		{
			return FalseNorthing;
		}
	};
}
#endif //PROJECTION_H
