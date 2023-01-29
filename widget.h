#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Compatibility>

#include "camera.h"
#include "surface.h"

#define GL_MATRIX_SIZE  16

//  globals
static double gMatrix[]={1,0,0,0,0,1,0,1.0,0,0,1,1.0,0,0,0,1};

class MainWindow;
class Widget : public QOpenGLWidget, protected QOpenGLFunctions_3_2_Compatibility
{
    Q_OBJECT

public:
    typedef double GL_Matrix[16];

    friend class MainWindow;
    explicit Widget(QWidget *parent = nullptr);
    ~Widget()override;

    void keyPressEvent(QKeyEvent * event) override;

private:
    //  graphic functions
    void drawSurface();
    void drawAxises();

    void initializeGL() override;
    void resizeGL(int w,int h) override;
    void paintGL() override;

private:
    //  visual components
    Surface * m_surface;
    Camera * m_camera;
};

#endif // WIDGET_H
