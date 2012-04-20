#include "glwidget.h"

#include <QGLShader>
#include <QGLShaderProgram>
#include <QMatrix4x4>

#include "scene.h"
#include "sceneparser.h"

GLWidget::GLWidget(QWidget *parent) :
  QGLWidget(parent),
  _timer(),
  _qtimer(this),
  _inputState(),
  _scene(NULL)
{
  connect(&_qtimer, SIGNAL(timeout()), this, SLOT(animate()));
  _qtimer.start(0.01);

  _timer.start();
}

GLWidget::~GLWidget()
{
  if(_scene) {
    delete _scene;
  }
}

void GLWidget::animate()
{
  _timer.update();

  _inputState.update();
  if(_scene) {
    _scene->update(_inputState, _timer.elapsed());
  }

  updateGL();
}

void GLWidget::renderMode(RenderMode mode) {
  GLenum glmode;
  switch(mode) {
  case Points:
    glmode = GL_POINT;
    break;
  case Wireframe:
    glmode = GL_LINE;
    break;
  case Solid:
    glmode = GL_FILL;
    break;
  }

  glPolygonMode(GL_FRONT, glmode);
}

void GLWidget::initializeGL() {
  qDebug("OpenGL version available: %s", glGetString(GL_VERSION));
  qDebug("GLSL version available: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

  glClearColor(.2, .2, .6, 1.0);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  _scene = SceneParser::load("/home/julio/CG/Projeto3/renderer/scenes/test.txt");
}

void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(_scene) {
    _scene->draw();
  }
}

void GLWidget::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
  rebuildProjection();
}

void GLWidget::mousePressEvent(QMouseEvent* event) {
  QPointF point = normalizePoint(QPointF(event->x(), event->y()));
  _inputState.mousePosition(point);

  if(event->button() == Qt::MiddleButton) {
    _inputState.mouseButton(InputState::MouseMiddle, true);
  }
  if(event->button() == Qt::LeftButton) {
    _inputState.mouseButton(InputState::MouseLeft, true);
  }
  if(event->button() == Qt::RightButton) {
    _inputState.mouseButton(InputState::MouseRight, true);
  }
  event->accept();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event) {
  QPointF point = normalizePoint(QPointF(event->x(), event->y()));
  _inputState.mousePosition(point);

  if(event->button() == Qt::MiddleButton) {
    _inputState.mouseButton(InputState::MouseMiddle, false);
  }
  if(event->button() == Qt::LeftButton) {
    _inputState.mouseButton(InputState::MouseLeft, false);
  }
  if(event->button() == Qt::RightButton) {
    _inputState.mouseButton(InputState::MouseRight, false);
  }
  event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event) {
  QPointF point = normalizePoint(QPointF(event->x(), event->y()));
  _inputState.mousePosition(point);
  event->accept();
}

void GLWidget::rebuildProjection() {
  float ratio = (float)width()/(float)height();
  if(_scene) {
    _scene->projection(60.0, ratio, 1.0, 100.0);
  }
}

QPointF GLWidget::normalizePoint(const QPointF &point) {
  float w, h ,x, y, g;

  w = width();
  h = height();
  g = h < w ? h : w;

  x = (2.0 * point.x() - w) / g;
  y = (h - 2.0 * point.y()) / g;

  return QPointF(x, y);
}
