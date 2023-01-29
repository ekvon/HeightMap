#include "widget.h"

#include <QKeyEvent>
#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLContext>

Widget::Widget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    //  default surface is used
    m_surface=new Surface();
    //  camera parameters are defined by surface parameters
    Range radiusRange(m_surface->width()*sqrt(2),0x2);
    Range heightRange(0,m_surface->width());
    Range phiRange(0,MATH_PI_RADIAN/2);
    //  camera creation
    m_camera=new Camera(radiusRange,heightRange,phiRange);
}

Widget::~Widget()
{
    if(m_surface)
        delete m_surface;
    if(m_camera)
        delete m_camera;
}

void Widget::initializeGL(){
    if(initializeOpenGLFunctions())
        printf("openGL functions are inited\n");
    else {
        printf("initialization is failed\n");
    }
    QSize size=this->size();
    GLdouble w=(GLdouble)size.width();
    GLdouble h=(GLdouble)size.height();
    printf("Widget (surface parameters): size.w=%f;size.h=%f;w=%d;h=%d\n",
           w,h,this->width(),this->height());
    glViewport(0,0,size.width(),size.height());
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f,0.2f,0.2f,0);

    glMatrixMode(GL_MODELVIEW);
    double m[16];
    //  obtain model-view matrix for current camera position
    m_camera->getModelView(m);
    glLoadMatrixd(m);

    glMatrixMode(GL_PROJECTION);
    //  in this case projection matrix is identity
    glOrtho(-1,1,-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);

    //  for 'DrawElement' function
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
}

void Widget::resizeGL(int w, int h){
    /*  update viewport parameters    */
    glViewport(0,0,w,h);
    glFlush();
}

void Widget::paintGL(){
    QSize size=this->size();
    GLdouble aspect=(GLdouble)size.width()/size.height();
    glViewport(0,0,size.width(),size.height());
    /*  just draw external sides   */
    drawSurface();
    drawAxises();
}

void Widget::drawAxises(){
    this->makeCurrent();

    /*  creation of Cartesian axises    */
    glBegin(GL_LINES);
        //  x
        glVertex3d(0,0,0);
        glVertex3d(1.0,0,0);
        //  y
        glVertex3d(0,0,0);
        glVertex3d(0,1.0,0);
        //  z
        glVertex3d(0,0,0);
        glVertex3d(0,0,1.0);
    glEnd();
    glFlush();
}
void Widget::drawSurface(){
    //  not realized yet
    int shapeX=m_surface->shape().width();
    int shapeZ=m_surface->shape().height();
    for(int x=0;x<shapeX;x++){
        for(int z=0;z<shapeZ;z++){
            Cell cell=m_surface->cell(x,z);
            if(x==0){
                glBegin(GL_LINE_LOOP);
                    //  draw all edges
                    glVertex3d(cell.m_pt0.x,cell.m_pt0.y,cell.m_pt0.z);
                    glVertex3d(cell.m_pt1.x,cell.m_pt1.y,cell.m_pt1.z);
                    glVertex3d(cell.m_pt2.x,cell.m_pt2.y,cell.m_pt2.z);
                    glVertex3d(cell.m_pt3.x,cell.m_pt3.y,cell.m_pt3.z);
                glEnd();
            }else if(z==0){
                glBegin(GL_LINE_STRIP);
                    //  draw only bottom, right and top edges
                    glVertex3d(cell.m_pt1.x,cell.m_pt1.y,cell.m_pt1.z);
                    glVertex3d(cell.m_pt2.x,cell.m_pt2.y,cell.m_pt2.z);
                    glVertex3d(cell.m_pt3.x,cell.m_pt3.y,cell.m_pt3.z);
                    glVertex3d(cell.m_pt0.x,cell.m_pt0.y,cell.m_pt0.z);
                glEnd();
            }else{
                glBegin(GL_LINE_STRIP);
                    //  draw only bottom and right edges
                    glVertex3d(cell.m_pt1.x,cell.m_pt1.y,cell.m_pt1.z);
                    glVertex3d(cell.m_pt2.x,cell.m_pt2.y,cell.m_pt2.z);
                    glVertex3d(cell.m_pt3.x,cell.m_pt3.y,cell.m_pt3.z);
                glEnd();
            }
        }
    }
    glFlush();
}
void Widget::keyPressEvent(QKeyEvent *event){
    double m[16];
    this->makeCurrent();
    glMatrixMode(GL_MODELVIEW);
    QWidget::keyPressEvent(event);
    switch (event->key()) {
    case(Qt::Key_Left):{
        m_camera->decPhi();
        m_camera->getModelView(m);
        glLoadMatrixd(m);
        break;
    }
    case(Qt::Key_Right):{
        m_camera->incPhi();
        m_camera->getModelView(m);
        glLoadMatrixd(m);
        break;
    }
    case(Qt::Key_Down):{
        m_camera->decH();
        //  math::cs::Spherical coord=m_camera->spherical();
        //  glRotated(MATH_PI_RADIAN/2-coord.theta,1,0,0);
        //  glRotated(coord.phi,0,1,0);
        m_camera->getModelView(m);
        glLoadMatrixd(m);
        break;
    }
    case(Qt::Key_Up):{
        m_camera->incH();
        //  math::cs::Spherical coord=m_camera->spherical();
        //  glRotated(MATH_PI_RADIAN/2-coord.theta,1,0,0);
        //  glRotated(coord.phi,0,1,0);
        m_camera->getModelView(m);
        glLoadMatrixd(m);
        break;
    }
    case(Qt::Key_A):{
        /*  approach    */
        m_camera->decR();
        m_camera->getModelView(m);
        glLoadMatrixd(m);
        //  math::cs::Cartesian coord=m_camera->cartesian().normalized();
        //  coord*=m_camera->deltaR();
        //  glTranslated(coord.x,coord.y,coord.z);
        break;
    }
    case(Qt::Key_D):{
        /*  */
        m_camera->incR();
        m_camera->getModelView(m);
        glLoadMatrixd(m);
        break;
    }
    default:{
        fprintf(stderr,"unreckognized character (%d)\n",event->key());
        break;
    }
    }
    //  QOpenGLContext * context=this->context();
    //  context->swapBuffers(context->surface());
    //  glFlush();
    this->update();
    this->makeCurrent();
}
