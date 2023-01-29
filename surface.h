#ifndef SURFACE_H
#define SURFACE_H

#include <QRectF>
#include <QSize>

#include "geometry.h"

#define SURFACE_DEFAULT_SCREEN_X 0.5
#define SURFACE_DEFAULT_SCREEN_Z 0.5
#define SURFACE_DEFAULT_SHAPE_X 0x10
#define SURFACE_DEFAULT_SHAPE_Z 0x10
//  max number of cells
#define SURFACE_MAX_SHAPE_VALUE 0x100

using namespace math;

class Surface:public QRectF
{
public:
    enum En_Axis
    {
        AXIS_X=0,
        AXIS_Y=1,
        AXIS_Z=2
    };
    Surface();
    //
    Surface(QSize &shape);
    //  world rect which will be mapped to the screen with the division shape
    Surface(const QRectF &world,QSize &shape);
    //  return extreme points associated with specified cells
    Cell cell(int cell_x,int cell_z)const;
    //
    QSize shape()const;

private:
    //  divide screen rect on cells
    void _divide();
    //
    void _setShape(QSize &shape);
    //  pointer to coord. of point with specified indices
    double * _pointPtr(int pt_x,int pt_z)const;
    //  return offset in coord. array of point with specified indices
    int _pointOffset(int pt_x,int pt_z)const;

private:
    //  world rect
    QRectF m_world;
    //  division of world rect in both directions
    QSize m_shape;
    //  screen parameters
    //  array to store coord. of points
    double * m_points;
};

#endif // SURFACE_H
