#include <QtWidgets>
#include <QtOpenGL>

#include "myglwidget.h"

#define ANGLE(x) 2 * M_PI * (x) / n
#define POINT(x, r, z) { \
    cos(ANGLE(x)) * (r), \
    sin(ANGLE(x)) * (r), \
    z }

#define VERTEX(p) glVertex3f(p.x, p.y, p.z)
#define NORM(p, q) glNormal3f((p.x + q.x) / 2, (p.y + q.y) / 2, (p.z + q.z) / 2)

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

void MyGLWidget::setTopRadius(double r)
{
    this->r2 = r;
    updateGL();
}

void MyGLWidget::setBottomRadius(double r)
{
    this->r1 = r;
    updateGL();
}

void MyGLWidget::setN(int n)
{
    this->n = n;
    updateGL();
}

void MyGLWidget::setHeight(double h)
{
    this->h = h;
    updateGL();
}

void MyGLWidget::setTransparent(bool val)
{
    transp = val;
    updateGL();
}

double MyGLWidget::getTopRaius() const
{
    return r2;
}

double MyGLWidget::getBottomRadius() const
{
    return r1;
}

int MyGLWidget::getN() const
{
    return n;
}

bool MyGLWidget::getTransparent() const
{
    return transp;
}

double MyGLWidget::getHeight() const
{
    return h;
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
        angle += 360 * 2;
    while (angle > 360)
        angle -= 360 * 2;
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
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    static GLfloat lightPosition[4] = { 1, 1, 1, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 2, 1.0, 0.0, 0.0);
    glRotatef(yRot / 2, 0.0, 1.0, 0.0);
    glRotatef(zRot / 2, 0.0, 0.0, 1.0);
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

    setXRotation(xRot + dy);
    setYRotation(yRot + dx);

    lastPos = event->pos();
}

struct Vertex {
    double x, y, z;
};

void MyGLWidget::draw()
{
    void (*f)(GLenum cap) = transp ? glDisable : glEnable;
    f(GL_DEPTH_TEST);

    QVector<Vertex> base, header;
    base.push_back(POINT(0, r1, 0));
    header.push_back(POINT(0, r2, h));
    for (int i = 1; i <= n; i++) {
        const Vertex& p1 = base.last();
        Vertex p2 = POINT(i, r1, 0);
        base.push_back(p2);

        const Vertex& p3 = header.last();
        Vertex p4 = POINT(i, r2, h);
        header.push_back(p4);

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

    glBegin(GL_POLYGON);
        glNormal3f(0, 0, 1);
        for (const Vertex& p : header) {
            VERTEX(p);
        }
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(0, 0, -1);
        for (const Vertex& p : base) {
            VERTEX(p);
        }
    glEnd();
    qglColor(Qt::red);
}
