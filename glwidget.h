#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QMouseEvent>
#include <QTimer>
#include <QGLWidget>

#include "inputstate.h"

class Scene;

class GLWidget : public QGLWidget {
  Q_OBJECT

public:
  enum RenderMode {
    Points,
    Wireframe,
    Solid
  };

  explicit GLWidget(QWidget *parent = 0);
  virtual ~GLWidget();

  void loadScene(const char* filename);

  void renderMode(RenderMode mode);

public slots:
  void animate();

protected:
  QTimer _qtimer;

  InputState _inputState;

  Scene *_scene;

  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);

  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);

  void rebuildProjection();

  QPointF normalizePoint(const QPointF& point);
};

#endif // GLWIDGET_H
