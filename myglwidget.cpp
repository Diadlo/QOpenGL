// myglwidget.cpp

#include <QtWidgets>
#include <QtOpenGL>

#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
}

MyGLWidget::~MyGLWidget()
{
}

QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void MyGLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::initializeGL()
{
    qglClearColor(Qt::black);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    static GLfloat lightPosition[4] = { 1, 1, 1, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    static GLfloat lightPosition2[4] = { -1, -1, -1, 0 };
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 16, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16, 0.0, 0.0, 1.0);
    draw();
}

void MyGLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

struct Point {
    double x, y, z;
};

void MyGLWidget::draw()
{
    double r1 = 1;
    double r2 = 0.5;
    int n = 5;
    double h = 1;

    QVector<Point> base, header;
#define ANGLE(x) 2 * M_PI * (x) / n
#define POINT(x, r, z) { \
    cos(ANGLE(x)) * (r), \
    sin(ANGLE(x)) * (r), \
    z }

    base.push_back(POINT(0, r1, 0));
    header.push_back(POINT(0, r2, h));
    for (int i = 1; i <= n; i++) {
        const Point& p1 = base.last();
        Point p2 = POINT(i, r1, 0);
        base.push_back(p2);

        const Point& p3 = header.last();
        Point p4 = POINT(i, r2, h);
        header.push_back(p4);

#define VERTEX(p) glVertex3f(p.x, p.y, p.z)
#define NORM(p, q) glNormal3f((p.x + q.x) / 2, (p.y + q.y) / 2, (p.z + q.z) / 2)
        glBegin(GL_QUADS);
        {
            NORM(p1, p2);
            VERTEX(p1);
            VERTEX(p2);
            VERTEX(p4);
            VERTEX(p3);
        }
        glEnd();
    }


    qglColor(Qt::red);
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, 1);
        for (const Point& p : header) {
            VERTEX(p);
        }
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(0, 0, 1);
        for (const Point& p : base) {
            VERTEX(p);
        }
    glEnd();

}
