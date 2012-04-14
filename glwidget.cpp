#include "glwidget.h"

#include <QTimer>
#include <QGLShader>
#include <QGLShaderProgram>

#include "mesh.h"
#include "meshloaderoff.h"
#include "meshloaderply.h"
#include "trackball.h"

GLWidget::GLWidget(QWidget *parent) :
  QGLWidget(parent),
  _vertShader(NULL),
  _fragShader(NULL),
  _shaderProgram(NULL),
  _mesh(NULL),
  _trackball(),
  _lightTrackball(),
  _camera(),
  _projectionMode(Perspective)
{
  // Set the animation timer
  _qtimer = new QTimer(this);
  connect(_qtimer, SIGNAL(timeout()), this, SLOT(animate()));
  _qtimer->start(0.01);

  _light.position = QVector4D(30, 30, 30, 0.0);
  _light.material.ambient = QVector4D(0.4, 0.4, 0.4, 1.0);
  _light.material.diffuse = QVector4D(1.0, 1.0, 1.0, 1.0);
  _light.material.specular = QVector4D(1.0, 1.0, 1.0, 1.0);

  _material.ambient = QVector4D(0.1, 0.1, 0.1, 1.0);
  _material.diffuse = QVector4D(1.0, 1.0, 1.0, 1.0);
  _material.specular = QVector4D(1.0, 1.0, 1.0, 1.0);
  _material.shininess = 10000.0;
}

GLWidget::~GLWidget()
{
  if(_mesh) {
    delete _mesh;
  }

  if(_shaderProgram) {
    delete _shaderProgram;
  }

  if(_vertShader) {
    delete _vertShader;
  }

  if(_fragShader) {
    delete _fragShader;
  }

  if(_qtimer) {
    delete _qtimer;
  }
}

void GLWidget::animate()
{
  updateGL();
}

void GLWidget::loadOFF(const char *filename) {
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
  MeshData *meshData = MeshLoaderPLY::load(filename);
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
  glClearColor(.2, .2, .6, 1.0);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  setShaders(":/shaders/phong/vshader.glsl", ":/shaders/phong/fshader.glsl");

  //loadOFF("/home/julio/CG/Projeto2/models/homer.off");
  //loadPLY("/home/julio/CG/Modelos/dragon_recon/dragon_vrip_res3.ply");
  loadPLY("/home/julio/CG/Modelos/bunny/reconstruction/bun_zipper.ply");
}

void GLWidget::paintGL() {
  if(!_shaderProgram || !_shaderProgram->isLinked())
    return;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _shaderProgram->bind();

  QVector4D ambient_product = _light.material.ambient* _material.ambient;
  QVector4D diffuse_product = _light.material.diffuse * _material.diffuse;
  QVector4D specular_product = _light.material.specular * _material.specular;

  QVector4D lightPosition = _lightTrackball.rotation().rotatedVector(_light.position.toVector3D());

  _shaderProgram->setUniformValue("LightPosition",lightPosition);
  _shaderProgram->setUniformValue("AmbientProduct",ambient_product);
  _shaderProgram->setUniformValue("DiffuseProduct",diffuse_product);
  _shaderProgram->setUniformValue("SpecularProduct",specular_product);
  _shaderProgram->setUniformValue("Shininess",_material.shininess);

  QMatrix4x4 view, model, mview, mvproj;

  view.setToIdentity();
  if(_projectionMode == Perspective) {
    QVector3D eye(0.0, 0.0, 2.4);
    QVector3D center(0.0, 0.0, 0.0);
    QVector3D up(0.0, 1.0, 0.0);
    view.lookAt(eye, center, up);
  }
  view.translate(_camera.translation());

  model.setToIdentity();
  model.rotate(_trackball.rotation());
  model.scale(_camera.scale());

  mview = view * model;
  mvproj = _projection * mview;

  _shaderProgram->setUniformValue("ModelMatrix", model);
  _shaderProgram->setUniformValue("ViewMatrix", view);
  _shaderProgram->setUniformValue("ProjectionMatrix", _projection);
  _shaderProgram->setUniformValue("ModelViewMatrix", mview);
  _shaderProgram->setUniformValue("ModelViewProjectionMatrix", mvproj);
  _shaderProgram->setUniformValue("NormalMatrix", mview.normalMatrix());

  if(_mesh) {
    _mesh->draw(_shaderProgram);
  }
}

void GLWidget::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
  rebuildProjection();
}

void GLWidget::mousePressEvent(QMouseEvent* event) {
  QPointF point = normalizePoint(QPointF(event->x(), event->y()));
  if(event->button() == Qt::MiddleButton) {
    _camera.press(point);
  }
  if(event->button() == Qt::LeftButton) {
    _trackball.press(point);
  }
  if(event->button() == Qt::RightButton) {
    _lightTrackball.press(point);
  }
  event->accept();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event) {
  QPointF point = normalizePoint(QPointF(event->x(), event->y()));
  if(event->button() == Qt::MiddleButton) {
    _camera.release(point);
  }
  if(event->button() == Qt::LeftButton) {
    _trackball.release(point);
  }
  if(event->button() == Qt::RightButton) {
    _lightTrackball.release(point);
  }
  event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event) {
  QPointF point = normalizePoint(QPointF(event->x(), event->y()));
  _trackball.move(point);
  _lightTrackball.move(point);
  _camera.move(point);
  event->accept();
}

void GLWidget::wheelEvent(QWheelEvent* event) {
  _camera.zoom(event->delta());
  event->accept();
}

void GLWidget::setProjection(ProjectionMode mode) {
  _projectionMode = mode;
  rebuildProjection();
}

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

void GLWidget::centerView() {
  _trackball.reset();
  _lightTrackball.reset();
  _camera.reset();
}

void GLWidget::rebuildProjection() {
  float ratio = (float)width()/(float)height();
  _projection.setToIdentity();
  switch(_projectionMode) {
  case Perspective:
    _projection.perspective(60.0, ratio, 1.0, 100);
    break;
  case Orthogonal:
    // TODO: These values should be based on the aspect ratio
    float z = 1.2;
    float x = 1.2;
    float y = 1.2;
    _projection.ortho(-x,x, -y,y, -z,z);
    break;
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
