#include "geometry.h"

namespace math
{
    Point3d::Point3d(double x_,double y_,double z_):x(x_),y(y_),z(z_){
    }

    Point3d::Point3d(double * coord){
        this->x=*coord++;
        this->y=*coord++;
        this->z=*coord;
    }

    double Point3d::operator[](size_t i) const{
        if(!i)
            return x;
        else if(i==1)
            return y;
        else {
            return z;
        }
    }

    double degree_to_radian(double angle){
        return angle*(MATH_PI_RADIAN/MATH_PI_DEGREE);
    }
    double radian_to_degree(double angle){
        return angle*(MATH_PI_DEGREE/MATH_PI_RADIAN);
    }
}
