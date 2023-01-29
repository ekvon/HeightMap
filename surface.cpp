#include "surface.h"

#include <stdlib.h>

Surface::Surface():QRectF(0,0,SURFACE_DEFAULT_SCREEN_X,SURFACE_DEFAULT_SCREEN_Z){
    //  world and screen rects are identical
    m_world.setX(0);
    m_world.setY(0);
    m_world.setWidth(1.0);
    m_world.setHeight(1.0);
    //  default shape
    m_shape.setWidth(SURFACE_DEFAULT_SHAPE_X);
    m_shape.setHeight(SURFACE_DEFAULT_SHAPE_Z);
    //
    this->_divide();
}

Surface::Surface(QSize &shape):QRectF(0,0,SURFACE_DEFAULT_SCREEN_X,SURFACE_DEFAULT_SCREEN_Z){
    //  world and screen rects are identical
    m_world.setX(0);
    m_world.setY(0);
    m_world.setWidth(1.0);
    m_world.setHeight(1.0);
    //  set specified shape if it's valid
    this->_setShape(shape);
}
//  map world rect on screen rect and apply specified shape
Surface::Surface(const QRectF& world,QSize &shape):QRectF(0,0,SURFACE_DEFAULT_SCREEN_X,SURFACE_DEFAULT_SCREEN_Z){
    m_world=world;
    //  scale screen rect to world rect keeping aspect ratio
    QSizeF size=this->size();
    size.scale(world.width(),world.height(),Qt::KeepAspectRatio);
    this->setSize(size);
    this->_setShape(shape);
}

void Surface::_divide()
{
    QSizeF size=this->size();
    double strideX=size.width()/m_shape.width();
    double strideZ=size.height()/m_shape.height();
    int pointsNum=(m_shape.width()+1)*(m_shape.height()+1);
    //  Initialization of points coord. Each point has three coord. of type double.
    m_points=new double[pointsNum*3];
    for(int x=0;x<=m_shape.width();x++){
        for(int z=0;z<=m_shape.height();z++){
            size_t off=(x*(m_shape.height()+1)+z)*3;
            //  x
            *(m_points+off)=this->topLeft().x()+x*strideX;
            //  y (altitude)
            *(m_points+off+1)=/*((double)rand())/(4*RAND_MAX)*/0;
            //  z
            *(m_points+off+2)=this->topLeft().y()+z*strideZ;
        }
    }
}

Cell Surface::cell(int cell_x, int cell_z) const{
    /*
     * Here 'cell_x' and 'cell_z' are coord. of cell. They MUST be in range [0,SURFACE_MAX_CELLS).
     * Each cell has four extreme points.
     */
    Cell cell;
    //  the first extreme points has offset
    double * coord=this->_pointPtr(cell_x,cell_z);
    cell.m_pt0=Point3d(coord);
    coord=this->_pointPtr(cell_x+1,cell_z);
    cell.m_pt1=Point3d(coord);
    coord=this->_pointPtr(cell_x+1,cell_z+1);
    cell.m_pt2=Point3d(coord);
    coord=this->_pointPtr(cell_x,cell_z+1);
    cell.m_pt3=Point3d(coord);
    return cell;
}

QSize Surface::shape() const{
    return m_shape;
}

void Surface::_setShape(QSize &shape){
    QSize maxShape(SURFACE_MAX_SHAPE_VALUE,SURFACE_MAX_SHAPE_VALUE);
    shape.boundedTo(maxShape);
    m_shape=shape;
}

double * Surface::_pointPtr(int pt_x, int pt_z) const{
    int offset=this->_pointOffset(pt_x,pt_z);
    if(0<=offset)
        return m_points+this->_pointOffset(pt_x,pt_z);
    else{
        //  invalid point indices
        return 0;
    }
}

int Surface::_pointOffset(int pt_x, int pt_z) const{
    if(0<=pt_x&&pt_x<=m_shape.width()&&0<=pt_z&&pt_z<=m_shape.height())
        return (pt_x*(m_shape.height()+1)+pt_z)*3;
    else {
        //  invalid point indices
        return -1;
    }
}
