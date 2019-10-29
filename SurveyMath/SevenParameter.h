#ifndef SEVENPARAMETER_H
#define SEVENPARAMETER_H
#include "SurveyMath\SurveyMath.h"
#include "SurveyMath\BasePoint.h"
#include "Eigen\Dense"
#include <QDebug>
using namespace std;
namespace SurveyMath {
    class SevenParameter
    {
    public:
        SevenParameter()
        {
            DX = 0;
            DY = 0;
            DZ = 0;
            RX = 0;
            RY = 0;
            RZ = 0;
            K = 1;
        }
        SevenParameter(double dx, double dy, double dz, double rx, double ry, double rz, double k)
        {
            DX = dx;
            DY = dy;
            DZ = dz;
            RX = rx;
            RY = ry;
            RZ = rz;
            K = k;
        }
        double DX;
        double DY;
        double DZ;
        double RX;//内部以弧度为单位
        double RY;//内部以弧度为单位
        double RZ;//内部以弧度为单位
        double K;//比例系数

        //逆参数
        double IDX;
        double IDY;
        double IDZ;
        double IRX;//内部以弧度为单位
        double IRY;//内部以弧度为单位
        double IRZ;//内部以弧度为单位
        double IK;//比例系数
        void SetSevenParameter(double dx, double dy, double dz, double secondrx, double  secondry, double  secondrz, double ppm)
        {
            DX = dx;
            DY = dy;
            DZ = dz;
            K = ppm / 1000000.0 + 1;
            RX = DMSToRadian(secondrx / 10000.0)*K;
            RY = DMSToRadian(secondry / 10000.0)*K;
            RZ = DMSToRadian(secondrz / 10000.0)*K;
        }
        double GetPPM()
        {
            return (K - 1)*1000000.0;

        }
        double GetSecondRX()
        {
            return RadianToDMS(RX / K) * 10000;
        }
        double GetSecondRY()
        {
            return RadianToDMS(RY / K) * 10000;
        }
        double GetSecondRZ()
        {
            return RadianToDMS(RZ / K) * 10000;
        }
        /**
        * 根据公共点进行七参数反解(布尔莎模型)
        *
        * @param srcPoint
        * @param tagPoint
        * @param SevenParameter
        * @return
        */
        bool Calculate(BasePoint *srcPoint, BasePoint *tagPoint,int pointCount)
        {
            CalculateD(srcPoint, tagPoint,pointCount);
            CalculateI(tagPoint,srcPoint,pointCount);
            return true;
        }
        BasePoint Transform(BasePoint srcPoint)
        {
            Eigen::MatrixXd A(3, 7);
            A(0, 0) = 1.0;
            A(0, 1) = 0.0;
            A(0, 2) = 0.0;
            A(0, 3) = srcPoint.x;
            A(0, 4) = 0.0;
            A(0, 5) = 0 - srcPoint.z;
            A(0, 6) = srcPoint.y;


            A(1, 0) = 0.0;
            A(1, 1) = 1.0;
            A(1, 2) = 0.0;
            A(1, 3) = srcPoint.y;
            A(1, 4) = srcPoint.z;
            A(1, 5) = 0.0;
            A(1, 6) = 0 - srcPoint.x;


            A(2, 0) = 0.0;
            A(2, 1) = 0.0;
            A(2, 2) = 1.0;
            A(2, 3) = srcPoint.z;
            A(2, 4) = 0 - srcPoint.y;
            A(2, 5) = srcPoint.x;
            A(2, 6) = 0.0;

            Eigen::MatrixXd X(7, 1);
            X(0, 0) = DX;
            X(1, 0) = DY;
            X(2, 0) = DZ;
            X(3, 0) = K;
            X(4, 0) = RX;
            X(5, 0) = RY;
            X(6, 0) = RZ;

            Eigen::MatrixXd B = A * X;
            double x = B(0, 0);
            double y = B(1, 0);
            double z = B(2, 0);
            BasePoint P(x, y, z);
            return P;
        }
        BasePoint InverseTransform(BasePoint srcPoint)
        {
            Eigen::MatrixXd A(3, 7);
            A(0, 0) = 1.0;
            A(0, 1) = 0.0;
            A(0, 2) = 0.0;
            A(0, 3) = srcPoint.x;
            A(0, 4) = 0.0;
            A(0, 5) = 0 - srcPoint.z;
            A(0, 6) = srcPoint.y;


            A(1, 0) = 0.0;
            A(1, 1) = 1.0;
            A(1, 2) = 0.0;
            A(1, 3) = srcPoint.y;
            A(1, 4) = srcPoint.z;
            A(1, 5) = 0.0;
            A(1, 6) = 0 - srcPoint.x;


            A(2, 0) = 0.0;
            A(2, 1) = 0.0;
            A(2, 2) = 1.0;
            A(2, 3) = srcPoint.z;
            A(2, 4) = 0 - srcPoint.y;
            A(2, 5) = srcPoint.x;
            A(2, 6) = 0.0;

            Eigen::MatrixXd X(7, 1);
            X(0, 0) = IDX;
            X(1, 0) = IDY;
            X(2, 0) = IDZ;
            X(3, 0) = IK;
            X(4, 0) = IRX;
            X(5, 0) = IRY;
            X(6, 0) = IRZ;

            Eigen::MatrixXd B = A * X;
            double x = B(0, 0);
            double y = B(1, 0);
            double z = B(2, 0);
            BasePoint P(x, y, z);
            return P;
        }
    private:
        /**
        * 根据公共点进行七参数反解(布尔莎模型),WGS84到目标
        *
        * @param srcPoint
        * @param tagPoint
        * @param SevenParameter
        * @return
        */
        bool CalculateD(BasePoint *srcPoint, BasePoint *tagPoint,int pointCount)
        {
            Eigen::MatrixXd A(pointCount * 3, 7);
            Eigen::MatrixXd B(pointCount * 3, 1);
            for (int i = 0; i < pointCount; i++) {
                // 构建参数矩阵
                A(i * 3, 0) = 1.0;
                A(i * 3, 1) = 0.0;
                A(i * 3, 2) = 0.0;
                A(i * 3, 3) = srcPoint[i].x;
                A(i * 3, 4) = 0.0;
                A(i * 3, 5) = 0 - srcPoint[i].z;
                A(i * 3, 6) = srcPoint[i].y;

                A(i * 3 + 1, 0) = 0.0;
                A(i * 3 + 1, 1) = 1.0;
                A(i * 3 + 1, 2) = 0.0;
                A(i * 3 + 1, 3) = srcPoint[i].y;
                A(i * 3 + 1, 4) = srcPoint[i].z;
                A(i * 3 + 1, 5) = 0.0;
                A(i * 3 + 1, 6) = 0 - srcPoint[i].x;


                A(i * 3 + 2, 0) = 0.0;
                A(i * 3 + 2, 1) = 0.0;
                A(i * 3 + 2, 2) = 1.0;
                A(i * 3 + 2, 3) = srcPoint[i].z;
                A(i * 3 + 2, 4) = 0 - srcPoint[i].y;
                A(i * 3 + 2, 5) = srcPoint[i].x;
                A(i * 3 + 2, 6) = 0.0;

                B(i * 3, 0) = tagPoint[i].x;
                B(i * 3 + 1, 0) = tagPoint[i].y;
                B(i * 3 + 2, 0) = tagPoint[i].z;

            }

            // 最小二乘法求解
            //cout << A;
            Eigen::Matrix<double, 7, 1> X = A.colPivHouseholderQr().solve(B);
            DX = X(0, 0);
            DY = X(1, 0);
            DZ = X(2, 0);
            RX = X(4, 0);
            RY = X(5, 0);
            RZ = X(6, 0);
            K = X(3, 0);
            return true;
        }
        /**
        * 根据公共点进行七参数反解(布尔莎模型)，目标到WGS84
        *
        * @param srcPoint
        * @param tagPoint
        * @param SevenParameter
        * @return
        */
        bool CalculateI(BasePoint *srcPoint, BasePoint *tagPoint,int pointCount)
        {
            Eigen::MatrixXd A(pointCount * 3, 7);
            Eigen::MatrixXd B(pointCount * 3, 1);
            for (int i = 0; i < pointCount; i++) {
                // 构建参数矩阵
                A(i * 3, 0) = 1.0;
                A(i * 3, 1) = 0.0;
                A(i * 3, 2) = 0.0;
                A(i * 3, 3) = srcPoint[i].x;
                A(i * 3, 4) = 0.0;
                A(i * 3, 5) = 0 - srcPoint[i].z;
                A(i * 3, 6) = srcPoint[i].y;

                A(i * 3 + 1, 0) = 0.0;
                A(i * 3 + 1, 1) = 1.0;
                A(i * 3 + 1, 2) = 0.0;
                A(i * 3 + 1, 3) = srcPoint[i].y;
                A(i * 3 + 1, 4) = srcPoint[i].z;
                A(i * 3 + 1, 5) = 0.0;
                A(i * 3 + 1, 6) = 0 - srcPoint[i].x;


                A(i * 3 + 2, 0) = 0.0;
                A(i * 3 + 2, 1) = 0.0;
                A(i * 3 + 2, 2) = 1.0;
                A(i * 3 + 2, 3) = srcPoint[i].z;
                A(i * 3 + 2, 4) = 0 - srcPoint[i].y;
                A(i * 3 + 2, 5) = srcPoint[i].x;
                A(i * 3 + 2, 6) = 0.0;

                B(i * 3, 0) = tagPoint[i].x;
                B(i * 3 + 1, 0) = tagPoint[i].y;
                B(i * 3 + 2, 0) = tagPoint[i].z;

            }

            // 最小二乘法求解
            //cout << A;
            Eigen::Matrix<double, 7, 1> X = A.colPivHouseholderQr().solve(B);
            IDX = X(0, 0);
            IDY = X(1, 0);
            IDZ = X(2, 0);
            IRX = X(4, 0);
            IRY = X(5, 0);
            IRZ = X(6, 0);
            IK = X(3, 0);
            return true;
        }

    };
}
#endif //SEVENPARAMETER_H
