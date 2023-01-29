#ifndef CAMERA_H
#define CAMERA_H

#include <QPair>

#include "geometry.h"

using namespace math;

class Camera
{
public:
    Camera(const Range &radiusRange,const Range &heightRange, const Range &phiRange);

    //  change camera position
    void incR();
    void decR();
    void incH();
    void decH();
    void incPhi();
    void decPhi();

    double deltaR()const;
    double deltaH()const;
    double deltaPhi()const;

    //  Copy current transformation matrix to specified buffer.
    //  The size of buffer MUST be greater than '16'.
    void getModelView(double * buf)const;
    //  accesors to camera coord.
    math::cs::Cylindrical cylindrical()const;
    math::cs::Spherical spherical()const;
    math::cs::Cartesian cartesian()const;

private:
    //  cylindrical coord. of camera in world frame
    math::cs::Cylindrical m_coord;

    double m_deltaR;
    double m_deltaH;
    double m_deltaPhi;

    //  transformation matrix connected with current camera position
    double m_modelView[16];

private:
    const Range m_radiusRange;
    const Range m_heightRange;
    const Range m_phiRange;
};

#endif // CAMERA_H
