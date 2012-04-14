#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QMouseEvent>
#include <QGLWidget>
#include <QMatrix4x4>

#include "camera.h"
#include "trackball.h"

class QGLShader;
class QGLShaderProgram;

class Mesh;

class GLWidget : public QGLWidget {
  Q_OBJECT

public:
  enum RenderMode {
    Points,
    Wireframe,
    Solid
  };

  enum ProjectionMode {
    Perspective,
    Orthogonal
  };

  explicit GLWidget(QWidget *parent = 0);
  virtual ~GLWidget();

  void loadOFF(const char* filename);

  void renderMode(RenderMode mode);

  void setProjection(ProjectionMode mode);
  bool setShaders(const char* vshader, const char* fshader);

  void centerView();

public slots:
  void animate();

protected:
  typedef struct {
    QVector4D ambient;
    QVector4D diffuse;
    QVector4D specular;
    float shininess;
  } tMaterial;

  typedef struct {
    QVector4D position;
    tMaterial material;
  } tLight;

  QTimer *_qtimer;

  // Shaders, materials and mesh should be in an "object" class

  QGLShader *_vertShader;
  QGLShader *_fragShader;
  QGLShaderProgram *_shaderProgram;

  tMaterial _material;
  tLight _light;

  Mesh *_mesh;

  Trackball _trackball;
  Trackball _lightTrackball;
  Camera _camera;

  ProjectionMode _projectionMode;

  QMatrix4x4 _projection;

  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);

  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void wheelEvent(QWheelEvent* event);

  void rebuildProjection();

  QPointF normalizePoint(const QPointF& point);
};

#endif // GLWIDGET_H
