#include "camera.h"
#include "matrix.h"

using namespace math;

Camera::Camera(const Range &radiusRange, const Range &heightRange, const Range &phiRange):
    //  default parameters
    m_radiusRange(radiusRange),
    m_heightRange(heightRange),
    m_phiRange(phiRange){
    //  initial coord.
    m_coord.r=m_radiusRange.second;
    m_coord.h=m_heightRange.first;
    m_coord.phi=m_phiRange.first;

    //  2% from radius range
    m_deltaR=(m_radiusRange.second-m_radiusRange.first)*0.02;
    //  2% from height range
    m_deltaH=(m_heightRange.second-m_heightRange.first)*0.02;
    //  1 degree
    m_deltaPhi=math::degree_to_radian(1.0);
}

void Camera::incR(){
    fprintf(stdout,"camera is incremented\n");
    if(m_coord.r<m_radiusRange.second)
        m_coord.r+=m_deltaR;
}

void Camera::decR(){
    fprintf(stdout,"camera is decremented\n");
    if(m_radiusRange.first<m_coord.r)
        m_coord.r-=m_deltaR;
}

void Camera::incH(){
    if(m_coord.h<m_heightRange.second)
        m_coord.h+=m_deltaH;
}

void Camera::decH(){
    if(m_heightRange.first<m_coord.h)
        m_coord.h-=m_deltaH;
}

void Camera::incPhi(){
    if(m_coord.phi<m_phiRange.second)
        m_coord.phi+=m_deltaPhi;
}

void Camera::decPhi(){
    if(m_phiRange.first<m_coord.phi)
        m_coord.phi-=m_deltaPhi;
}

double Camera::deltaR() const{
    return m_deltaR;
}

double Camera::deltaH() const{
    return m_deltaH;
}

double Camera::deltaPhi() const{
    return m_deltaPhi;
}

math::cs::Cylindrical Camera::cylindrical() const{
    return m_coord;
}

math::cs::Spherical Camera::spherical() const{
    return cs::convert<cs::Cylindrical,cs::Spherical>::make(this->m_coord);
}

math::cs::Cartesian Camera::cartesian() const{
    return cs::convert<cs::Cylindrical,cs::Cartesian>::make(this->m_coord);
}

void Camera::getModelView(double *buf) const{
    typedef Matrix<double> MatrixType;
    //  spherical coord. required for rotation angles definition
    cs::Spherical cs_sph=cs::convert<cs::Cylindrical,cs::Spherical>::make(this->m_coord);
    MatrixType rotY=rotationY<MatrixType>(/*MATH_PI_RADIAN+*/cs_sph.phi);
    MatrixType rotX=rotationX<MatrixType>(MATH_PI_RADIAN/2-cs_sph.theta);
    cs::Cartesian cs_cart=cs::convert<cs::Cylindrical,cs::Cartesian>::make(this->m_coord);
    MatrixType tr=translation<MatrixType>(-cs_cart.x,-cs_cart.y,-cs_cart.z);
    MatrixType result=rotY*rotX;
    result+=tr;
    //  load result matrix as raw data into the buffer
    result.getRaw(buf,16);

    //  debug-output
    fprintf(stdout,"camera spherical: r=%f,phi=%f,theta=%f\n",cs_sph.r,cs_sph.phi,cs_sph.theta);
    std::string str=rotY.toString();
    fprintf(stdout,"rotY:\n%s\n",str.c_str());
    str=rotX.toString();
    fprintf(stdout,"rotX:\n%s\n",str.c_str());
    fprintf(stdout,"cartesian: x=%f, y=%f, z=%f\n",cs_cart.x,cs_cart.y,cs_cart.z);
    str=result.toString();
    fprintf(stdout,"rot:\n%s\n",str.c_str());
    fprintf(stdout,"glBuffer: ");
    for(size_t i=0;i<16;i++){
        fprintf(stdout,"%f ",*(buf+i));
    }
    fprintf(stdout,"\n");
}
