#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QPair>

#include <math.h>

#define MATH_PI_RADIAN 3.1415926535897
#define MATH_PI_DEGREE 180.0

namespace math
{
    typedef QPair<double,double> Range;

    struct Point3d
    {
        Point3d(double x_=0,double y_=0,double z_=0);
        Point3d(double * coord);

        double operator[](size_t i)const;

        double x;
        double y;
        double z;
    };

    typedef Point3d Vector3d;

    struct Cell
    {
        Point3d m_pt0;
        Point3d m_pt1;
        Point3d m_pt2;
        Point3d m_pt3;
    };

    //  coord. systems namespace
    namespace cs {

        struct Cartesian
        {
            Cartesian(double x_=0,double y_=0,double z_=0):x(x_),y(y_),z(z_){}
            double r()const{
                return sqrt(x*x+y*y+z*z);
            }
            struct Cartesian normalized()const{
                double radius=this->r();
                Cartesian result;
                if(!radius)
                    return result;
                result.x=this->x;
                result.y=this->y;
                result.z=this->z;
                result*=1/radius;
                return result;
            }
            Cartesian& operator*=(double alpha){
                x*=alpha;
                y*=alpha;
                z*=alpha;
                return *this;
            }
            double x;
            double y;
            double z;
        };

        struct Spherical
        {
            double r;
            //  angle between projection of radius vector on Ozx-plane and z-axis
            double phi;
            //  angle between radius vector and y-axis
            double theta;
        };

        struct Cylindrical
        {
            double r;
            double h;
            //  angle between radius vector and z-axis
            double phi;
        };

        template <typename _Cs1,typename _Cs2> struct convert;

        template <>
        struct convert<Cylindrical,Cartesian>{
            static Cartesian make(const Cylindrical &src){
                Cartesian dst;
                dst.x=src.r*sin(src.phi);
                dst.y=src.h;
                dst.z=src.r*cos(src.phi);
                return dst;
            }
        };

        template <>
        struct convert<Cylindrical,Spherical>{
            static Spherical make(const Cylindrical &src){
                Spherical dst;
                dst.r=sqrt(src.r*src.r+src.h*src.h);
                dst.phi=src.phi;
                dst.theta=acos(src.h/dst.r);
                return dst;
            }
        };
    }
    double radian_to_degree(double angle);
    double degree_to_radian(double angle);
}

#endif // GEOMETRY_H
