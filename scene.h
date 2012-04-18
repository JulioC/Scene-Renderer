#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "camera.h"
#include "trackball.h"

class QGLShaderProgram;

class Light;

class Scene
{
public:
  Scene();
  virtual ~Scene();

  virtual void update(const InputState &state);
  virtual void draw(QGLShaderProgram *shaderProgram = NULL);

  //void addObject(Object *object);
  void addLight(Light *light);
  //void setBackground();

protected:
  QVector<Light*> _lights;
  QVector<Object*> _objects;

  Camera _camera;
  Trackball _trackball;
  Trackball _ambientTrackball;


};

#endif // SCENE_H
