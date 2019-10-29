#pragma once
#include "SurveyMath\surveymath.h"
#include "SurveyMath\CoordinateTransformation.h"
#include "Eigen\Dense"
#include <vector>
using namespace SurveyMath;
//大地坐标转换为空间直角坐标
GeocentricCoordinate CoordinateTransformation::GeodeticToGeocentric(Projection prj, GeodeticCoordinate GeodeticPoint)
{
	double X;//空间直角坐标系
	double Y;//空间直角坐标系
	double Z;//空间直角坐标系
	double B;//纬度度数
	double L;//经度度数
	double H;//大地高
	double a;//为椭球长半轴
	double e1;//第一偏心率	    	 
	double W;//中间参数  
	double N;//卯酉圈曲率半径     


	B = GeodeticPoint.x;//输入点的纬度
	L = GeodeticPoint.y;//输出点的经度
	H = GeodeticPoint.z;

	a = prj.GetDatum().GetLongAxis();
	e1 = prj.GetDatum().GetFirstEccentricity();
	W = sqrt(1.0 - pow(e1 * sin(B), 2.0));
	N = a / W;

	X = (N + H) * cos(B) * cos(L);
	Y = (N + H) * cos(B) * sin(L);
	Z = (N * (1.0 - e1 * e1) + H) * sin(B);

	BasePoint p(X, Y, Z);
	return p;
}
//空间直角坐标换为大地坐标
GeodeticCoordinate  CoordinateTransformation::GeocentricToGeodetic(Projection prj, GeocentricCoordinate GeocentricPoint)
{

	double X;//空间直角坐标系
	double Y;//空间直角坐标系
	double Z;//空间直角坐标系
	double B;//纬度度数
	double L;//经度度数
	double H;//大地高
	double a;//为椭球长半轴
	double b;//为椭球短半轴
	double e1;//第一偏心率	    	
	double e2;
	double W;//中间参数  
	double N;//卯酉圈曲率半径     
	double BT1;
	double BT2;
	double U;
	double B0;
	double H0;
	double H1;

	X = GeocentricPoint.x;
	Y = GeocentricPoint.y;
	Z = GeocentricPoint.z;

	a = prj.GetDatum().GetLongAxis();
	b = a - a * prj.GetDatum().GetFlattening();
	e1 = prj.GetDatum().GetFirstEccentricity();
	e2 = prj.GetDatum().GetSecondEccentricity();

	U = atan((Z * a) / (sqrt(X*X + Y * Y)*b));

	B0 = atan((Z + b * e2 * e2 * sin(U) * sin(U) * sin(U))
		/ (sqrt(X * X + Y * Y) - a * e1 * e1 * cos(U) * cos(U) * cos(U)));
	W = sqrt(1.0 - pow(e1 * sin(B0), 2.0));
	N = a / W;


	H0 = X * X + Y * Y;
	H1 = Z + N * e1 * e1 * sin(B0);
	H = sqrt(H0 + H1 * H1) - N;

	BT1 = Z / sqrt(X * X + Y * Y);
	BT2 = (1 - e1 * e1 * N / (N + H));
	B = atan(BT1 / BT2);

	L = atan(Y / X);
	if (L < 0) L = L + PI;

	BasePoint p(B, L, H);
	return p;
}

//大地坐标转平面坐标
CartesianCoordinate CoordinateTransformation::GeodeticToCartesian(Projection prj, GeodeticCoordinate  GeodeticPoint)
{
	double x;//迪卡尔坐标系
	double y;//迪卡尔坐标系
	double B;//纬度度数
	double L;//经度度数
	double L0;//中央经线度数
	double l;//L-L0
	double a;//为椭球长半轴
	double e1;//第一偏心率	    	 
	double e2;//第二偏心率
	double g;//中间参数η
	double t;//中间参数
	double m;//中间参数
	double W;//中间参数  
	double N;//卯酉圈曲率半径     
	double X;//子午线弧长
	double X1, X2, X3;//计算X的中间参数
	double Y1, Y2;//计算y的中间参数

	B = GeodeticPoint.x;//输入点的纬度
	L = GeodeticPoint.y;//输出点的经度

	L0 = prj.GetCentralMeridian();//中央子午线
	l = L - L0;
	a = prj.GetDatum().GetLongAxis();
	e1 = prj.GetDatum().GetFirstEccentricity();
	e2 = prj.GetDatum().GetSecondEccentricity();
	g = e2 * cos(B);
	t = tan(B);
	m = cos(B)*l;
	W = sqrt(1.0 - pow(e1 * sin(B), 2.0));
	N = a / W;


	X = X0(B, a, e1);

	X1 = N * t * pow(cos(B) * l, 2.0);
	X2 = (1.0 / 24.0) * (5.0 - pow(t, 2.0) + 9.0 * pow(g, 2.0)
		+ 4.0 * pow(g, 4.0)) * pow(cos(B) * l, 2.0);
	X3 = (1.0 / 720.0) * (61.0 - 58.0 * pow(t, 2.0) + pow(t, 4.0)) * pow(cos(B) * l, 4.0);

	x = X + X1 * (0.5 + X2 + X3);


	Y1 = (1.0 / 6.0) * (1.0 - pow(t, 2.0) + pow(g, 2.0)) * pow(m, 3.0);
	Y2 = (1.0 / 120.0) * (5.0 - 18.0 * pow(t, 2.0) + pow(t, 4.0) + 14.0 * pow(g, 2.0)
		- 58.0 * pow(g * t, 2.0)) * pow(m, 5.0);

	y = N * (m + Y1 + Y2);

	double px = x*prj.GetCentralMeridianScaleFactor() + prj.GetFalseNorthing();
	double py = y * prj.GetCentralMeridianScaleFactor() + prj.GetFalseEasting();

	CartesianCoordinate p(px, py, GeodeticPoint.z);

	return p;
}
//平面坐标转大地坐标
GeocentricCoordinate CoordinateTransformation::CartesianToGeodetic(Projection prj, CartesianCoordinate CartesianPoint)
{
			
	double x;//迪卡尔坐标系
	double y;//迪卡尔坐标系
	double B;//纬度度数
	double L;//经度度数
	double Bf = 0;//反算纬度时要用的中间值，底点纬度
	double tFB0, tFB1;//计算Bf要用的中间参数
	double L0;//中央经线度数
	double l;//L-L0
	double a;//为椭球长半轴
	double e1;//第一偏心率	    	 
	double e2;//第二偏心率
	double gf;//中间参数η
	double tf;//中间参数
	double Mf;//中间参数  
	double Wf;//中间参数
	double Nf;//卯酉圈曲率半径     
	double B1, B2, B3;//计算X的中间参数
	double L1, L2, L3;//计算y的中间参数
	double A0, A1, A2, A3, A4, A5, A6;//计算N的中间参数
	double K0, K2, K4, K6;

	x = CartesianPoint.x - prj.GetFalseNorthing();//减去北移量后的值
	x = x / prj.GetCentralMeridianScaleFactor();
	y = CartesianPoint.y - prj.GetFalseEasting();//减去东偏见移量后的值
	y = y / prj.GetCentralMeridianScaleFactor();

	L0 = prj.GetCentralMeridian();//中央子午线
	a = prj.GetDatum().GetLongAxis();
	e1 = prj.GetDatum().GetFirstEccentricity();
	e2 = prj.GetDatum().GetSecondEccentricity();



	A0 = 1.0 + (3.0 / 4.0) * pow(e1, 2.0) + (45.0 / 64.0) * pow(e1, 4.0) +
		(175.0 / 256.0) * pow(e1, 6.0) + (11025.0 / 16384.0) * pow(e1, 8.0)
		+ (43659.0 / 65536.0) * pow(e1, 10.0) + (693693.0 / 1048576.0) * pow(e1, 12.0);
	A1 = (3.0 / 8.0) * pow(e1, 2.0) + (15.0 / 32.0) * pow(e1, 4.0) +
		(525.0 / 1024.0) * pow(e1, 6.0) + (2205.0 / 4096.0) * pow(e1, 8.0)
		+ (72765.0 / 131072.0) * pow(e1, 10.0) + (297297.0 / 524288.0) * pow(e1, 12.0);
	A2 = (15.0 / 256.0) * pow(e1, 4.0) + (105.0 / 1024.0) * pow(e1, 6.0)
		+ (2205.0 / 16384.0) * pow(e1, 8.0) + (10395.0 / 65536.0) * pow(e1, 10.0)
		+ (1486485.0 / 8388608.0) * pow(e1, 12.0);
	A3 = (35.0 / 3072.0) * pow(e1, 6.0) + (105.0 / 4096.0) * pow(e1, 8.0)
		+ (10395.0 / 262144.0) * pow(e1, 10.0) + (55055.0 / 1048576.0) * pow(e1, 12.0);
	A4 = (315.0 / 131072.0) * pow(e1, 8.0) + (3465.0 / 524288.0) * pow(e1, 10.0)
		+ (99099.0 / 8388608.0) * pow(e1, 12.0);
	A5 = (693.0 / 1310720.0) * pow(e1, 10.0) + (9009.0 / 5242880.0) * pow(e1, 12.0);
	A6 = (1001.0 / 8388608.0) * pow(e1, 12.0);

	K0 = (1.0 / 2.0)*((3.0 / 4.0)*pow(e1, 2.0) + (45.0 / 64.0)*pow(e1, 4.0)
		+ (350.0 / 512.0)*pow(e1, 6.0) + (11025.0 / 16384.0)*pow(e1, 8.0));
	K2 = (-1.0 / 3.0)*((63.0 / 64.0)*pow(e1, 4.0) + (1108.0 / 512.0)*pow(e1, 6.0) + (58239.0 / 16384.0)*pow(e1, 8.0));
	K4 = (1.0 / 3.0) * ((604.0 / 512.0) * pow(e1, 6.0) + (68484.0 / 16384.0) * pow(e1, 8.0));
	K6 = (-1.0 / 3.0) * ((26328.0 / 16384.0) * pow(e1, 8.0));

	Bf = x / 10000000;
	//迭代法求底点纬度Bf
	while (true)
	{
		B = Bf;
		tFB0 = FB0(B, a, e1);
		tFB1 = FB1(B, a, e1);
		Bf = B + (x - tFB0) / tFB1;
		if (abs((B - Bf)) < 0.00000000000001) break;
	}

	gf = e2 * cos(Bf);
	tf = tan(Bf);
	Wf = sqrt(1.0 - pow(e1 * sin(Bf), 2.0));
	Mf = a * (1.0 - pow(e1, 2)) / pow(Wf, 3.0);
	Nf = a / Wf;

	B1 = (tf / (2.0 * Mf)) * y * (y / Nf);
	B2 = (1.0 / 12.0)*(5.0 + 3.0*pow(tf, 2.0) + pow(gf, 2.0) - 9.0*pow(gf*tf, 2.0)) *pow((y / Nf), 2.0);
	B3 = (1.0 / 360.0) * (61.0 + 90.0 * pow(tf, 2.0) + 45.0 * pow(tf, 4.0)) * pow((y / Nf), 4.0);
	B = Bf - B1 * (1.0 - B2 + B3);

	L1 = y / (cos(Bf) * Nf);
	L2 = (1.0 / 6.0) * (1.0 + 2.0 * pow(tf, 2.0) + pow(gf, 2.0)) * pow((y / Nf), 2.0);
	L3 = (1.0 / 120.0)*(5.0 + 28.0*pow(tf, 2.0) + 24.0*pow(tf, 4.0)
		+ 6.0*pow(gf, 2.0) + 8.0*pow(gf*tf, 2.0)) *pow((y / Nf), 4.0);
	l = L1 * (1 - L2 + L3);
	L = L0 + l;

	double px = B;
	double py =L;

	GeocentricCoordinate p(px, py, CartesianPoint.z);
	return p;
}
//高斯坐标正算
CartesianCoordinate CoordinateTransformation::PositiveGauss(Projection prj, GeodeticCoordinate GeodeticPoint)
{
	return GeodeticToCartesian(prj, GeodeticPoint);
}
//高斯坐标反算
GeocentricCoordinate CoordinateTransformation::InverseGauss(Projection prj, CartesianCoordinate CartesianPoint)
{
	return CartesianToGeodetic(prj, CartesianPoint);
}
//UTM大地坐标转平面坐标
CartesianCoordinate CoordinateTransformation::PositiveUTM(GeodeticCoordinate GeodeticPoint)
{
	double L = GeodeticPoint.y;
	double B = GeodeticPoint.x;
	double dN = 0.0;
	if (B < 0)
	{
		dN = 10000000.0;
	}

	int iZone = static_cast<int>((RadianToDegree(L) + 180.0) / 6) + 1;
	double dC = DegreeToRadian(-183.0 + (iZone * 6.0));
	Ellipsoid wgs84(6378137.0, 1 / 298.257223563);
	Projection prj_utm(wgs84, 0.9996, dC, 0, 500000, dN);
	return GeodeticToCartesian(prj_utm, GeodeticPoint);
}
//UTM平面坐标转大地坐标
GeocentricCoordinate CoordinateTransformation::InverseUTM(int Zone, bool IsSouth, CartesianCoordinate CartesianPoint)
{
	double dN = 0.0;
	if (IsSouth)
	{
		dN = 10000000.0;
	}
	Ellipsoid wgs84(6378137.0, 1 / 298.257223563);
	double dC = DegreeToRadian(-183.0 + (Zone * 6.0));
	Projection prj_utm(wgs84, 0.9996, dC, 0, 500000, dN);
	return CartesianToGeodetic(prj_utm, CartesianPoint);
}
double CoordinateTransformation::X0(double B, double a, double e1)
{
	double X, A0, A1, A2, A3, A4, A5, A6;//计算N的中间参数
	A0 = 1.0 + (3.0 / 4.0) * pow(e1, 2.0) + (45.0 / 64.0) * pow(e1, 4.0) +
		(175.0 / 256.0) * pow(e1, 6.0) + (11025.0 / 16384.0) * pow(e1, 8.0)
		+ (43659.0 / 65536.0) * pow(e1, 10.0) + (693693.0 / 1048576.0) * pow(e1, 12.0);
	A1 = (3.0 / 8.0) * pow(e1, 2.0) + (15.0 / 32.0) * pow(e1, 4.0) +
		(525.0 / 1024.0) * pow(e1, 6.0) + (2205.0 / 4096.0) * pow(e1, 8.0)
		+ (72765.0 / 131072.0) * pow(e1, 10.0) + (297297.0 / 524288.0) * pow(e1, 12.0);
	A2 = (15.0 / 256.0) * pow(e1, 4.0) + (105.0 / 1024.0) * pow(e1, 6.0)
		+ (2205.0 / 16384.0) * pow(e1, 8.0) + (10395.0 / 65536.0) * pow(e1, 10.0)
		+ (1486485.0 / 8388608.0) * pow(e1, 12.0);
	A3 = (35.0 / 3072.0) * pow(e1, 6.0) + (105.0 / 4096.0) * pow(e1, 8.0)
		+ (10395.0 / 262144.0) * pow(e1, 10.0) + (55055.0 / 1048576.0) * pow(e1, 12.0);
	A4 = (315.0 / 131072.0) * pow(e1, 8.0) + (3465.0 / 524288.0) * pow(e1, 10.0)
		+ (99099.0 / 8388608.0) * pow(e1, 12.0);
	A5 = (693.0 / 1310720.0) * pow(e1, 10.0) + (9009.0 / 5242880.0) * pow(e1, 12.0);
	A6 = (1001.0 / 8388608.0) * pow(e1, 12.0);

	X = a * (1.0 - pow(e1, 2.0)) * (A0 * B - A1 * sin(2.0 * B) + A2 * sin(4.0 * B)
		- A3 * sin(6.0 * B) + A4 * sin(8.0 * B) - A5 * sin(10.0 * B) + A6 * sin(12.0 * B));

	return X;
}
//子午线弧长
double CoordinateTransformation::FB0(double B, double a, double e1)
{
	double F, A0, A1, A2, A3, A4, A5, A6;//计算N的中间参数

	A0 = 1.0 + (3.0 / 4.0) * pow(e1, 2.0) + (45.0 / 64.0) * pow(e1, 4.0) +
		(175.0 / 256.0) * pow(e1, 6.0) + (11025.0 / 16384.0) * pow(e1, 8.0)
		+ (43659.0 / 65536.0) * pow(e1, 10.0) + (693693.0 / 1048576.0) * pow(e1, 12.0);
	A1 = (3.0 / 8.0) * pow(e1, 2.0) + (15.0 / 32.0) * pow(e1, 4.0) +
		(525.0 / 1024.0) * pow(e1, 6.0) + (2205.0 / 4096.0) * pow(e1, 8.0)
		+ (72765.0 / 131072.0) * pow(e1, 10.0) + (297297.0 / 524288.0) * pow(e1, 12.0);
	A2 = (15.0 / 256.0) * pow(e1, 4.0) + (105.0 / 1024.0) * pow(e1, 6.0)
		+ (2205.0 / 16384.0) * pow(e1, 8.0) + (10395.0 / 65536.0) * pow(e1, 10.0)
		+ (1486485.0 / 8388608.0) * pow(e1, 12.0);
	A3 = (35.0 / 3072.0) * pow(e1, 6.0) + (105.0 / 4096.0) * pow(e1, 8.0)
		+ (10395.0 / 262144.0) * pow(e1, 10.0) + (55055.0 / 1048576.0) * pow(e1, 12.0);
	A4 = (315.0 / 131072.0) * pow(e1, 8.0) + (3465.0 / 524288.0) * pow(e1, 10.0)
		+ (99099.0 / 8388608.0) * pow(e1, 12.0);
	A5 = (693.0 / 1310720.0) * pow(e1, 10.0) + (9009.0 / 5242880.0) * pow(e1, 12.0);
	A6 = (1001.0 / 8388608.0) * pow(e1, 12.0);

	F = a * (1.0 - pow(e1, 2.0)) * (A0 * B - A1 * sin(2.0 * B)
		+ A2 * sin(4.0 * B) - A3 * sin(6.0 * B) + A4 * sin(8.0 * B)
		- A5 * sin(10.0 * B) + A6 * sin(12.0 * B));
	return F;
}

//底点纬度用到的函数
double CoordinateTransformation::FB1(double B, double a, double e1)
{
	double F, A0, A1, A2, A3, A4, A5, A6;//计算N的中间参数

	A0 = 1.0 + (3.0 / 4.0) * pow(e1, 2.0) + (45.0 / 64.0) * pow(e1, 4.0) +
		(175.0 / 256.0) * pow(e1, 6.0) + (11025.0 / 16384.0) * pow(e1, 8.0)
		+ (43659.0 / 65536.0) * pow(e1, 10.0) + (693693.0 / 1048576.0) * pow(e1, 12.0);
	A1 = (3.0 / 8.0) * pow(e1, 2.0) + (15.0 / 32.0) * pow(e1, 4.0) +
		(525.0 / 1024.0) * pow(e1, 6.0) + (2205.0 / 4096.0) * pow(e1, 8.0)
		+ (72765.0 / 131072.0) * pow(e1, 10.0) + (297297.0 / 524288.0) * pow(e1, 12.0);
	A2 = (15.0 / 256.0) * pow(e1, 4.0) + (105.0 / 1024.0) * pow(e1, 6.0)
		+ (2205.0 / 16384.0) * pow(e1, 8.0) + (10395.0 / 65536.0) * pow(e1, 10.0)
		+ (1486485.0 / 8388608.0) * pow(e1, 12.0);
	A3 = (35.0 / 3072.0) * pow(e1, 6.0) + (105.0 / 4096.0) * pow(e1, 8.0)
		+ (10395.0 / 262144.0) * pow(e1, 10.0) + (55055.0 / 1048576.0) * pow(e1, 12.0);
	A4 = (315.0 / 131072.0) * pow(e1, 8.0) + (3465.0 / 524288.0) * pow(e1, 10.0)
		+ (99099.0 / 8388608.0) * pow(e1, 12.0);
	A5 = (693.0 / 1310720.0) * pow(e1, 10.0) + (9009.0 / 5242880.0) * pow(e1, 12.0);
	A6 = (1001.0 / 8388608.0) * pow(e1, 12.0);
	F = a * (1.0 - pow(e1, 2.0)) * (A0 - 2.0 * A1 * cos(2.0 * B)
		+ 4.0 * A2 * cos(4.0 * B) - 6.0 * A3 * cos(6.0 * B)
		+ 8.0 * A4 * cos(8.0 * B) - 10.0 * A5 * cos(10.0 * B)
		+ 12.0 * A6 * cos(12.0 * B));
	return F;
}



