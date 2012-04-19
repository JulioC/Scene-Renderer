#include "glwidget.h"

#include <QGLShader>
#include <QGLShaderProgram>
#include <QMatrix4x4>

#include "scene.h"
#include "sceneparser.h"

GLWidget::GLWidget(QWidget *parent) :
  QGLWidget(parent),
  _qtimer(this),
  _inputState(),
  _scene(NULL)
{
  connect(&_qtimer, SIGNAL(timeout()), this, SLOT(animate()));
  _qtimer.start(0.01);
}

GLWidget::~GLWidget()
{
  if(_scene) {
    delete _scene;
  }
}

void GLWidget::animate()
{
  _inputState.update();
  if(_scene) {
    _scene->update(_inputState);
  }

  updateGL();
}

/*void GLWidget::loadOFF(const char *filename) {
  MeshData *meshData = MeshLoaderOFF::load(filename);
  if(!meshData) {
    qDebug("Failed to load OFF file %s", filename);
    return;
  }

  meshData->normalize();
  meshData->computeNormals();

  if(_mesh) {
    delete _mesh;
  }
  _mesh = new Mesh(meshData);
  delete meshData;
}

void GLWidget::loadPLY(const char *filename) {
  /*MeshData *meshData = MeshLoaderPLY::load(filename);
  if(!meshData) {
    qDebug("Failed to load PLY file %s", filename);
    return;
  }

  meshData->normalize();
  meshData->computeNormals();

  if(_mesh) {
    delete _mesh;
  }
  _mesh = new Mesh(meshData);
  delete meshData;
}*/

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
  glClearColor(.2, .2, .6, 1.0);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // TODO: Load sample scene
  _scene = SceneParser::load("/home/julio/CG/Projeto3/Renderer/TestScene.txt");
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
/*
bool GLWidget::setShaders(const char* vshader, const char* fshader) {
  if(_shaderProgram) {
    delete _shaderProgram;
    _shaderProgram = NULL;
  }

  if(_vertShader) {
    delete _vertShader;
    _vertShader = NULL;
  }

  if(_fragShader) {
    delete _fragShader;
    _fragShader = NULL;
  }

  _vertShader = new QGLShader(QGLShader::Vertex);
  _fragShader = new QGLShader(QGLShader::Fragment);

  if(!_vertShader->compileSourceFile(vshader)) {
    qWarning() << _vertShader->log();
    return false;
  }

  if(!_fragShader->compileSourceFile(fshader)) {
    qWarning() << _fragShader->log();
    return false;
  }

  _shaderProgram = new QGLShaderProgram();
  _shaderProgram->addShader(_vertShader);
  _shaderProgram->addShader(_fragShader);

  if(!_shaderProgram->link()) {
    qWarning() << _shaderProgram->log();
    return false;
  }

  _shaderProgram->bind();

  return true;
}
*/
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
