#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include <QMatrix4x4>

#include "camera.h"
#include "inputstate.h"
#include "trackball.h"

class QGLShaderProgram;

class Light;
class Object;

class Scene
{
public:
  Scene();
  virtual ~Scene();

  virtual void update(const InputState &state, float delta);
  virtual void draw(QGLShaderProgram *shaderProgram = NULL);

  void projection(float fov, float ratio, float near, float far);

  void addObject(Object *object);
  void addLight(Light *light);
  //void setBackground();

protected:
  QVector<Light*> _lights;
  QVector<Object*> _objects;

  QMatrix4x4 _projection;
  QMatrix4x4 _view;

  Camera _camera;
  Trackball _trackball;

  void applyLights(QGLShaderProgram *shaderProgram);
};

#endif // SCENE_H
