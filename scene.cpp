#include "scene.h"

#include "light.h"
#include "object.h"

Scene::Scene() :
  _lights(),
  _objects(),
  _projection(),
  _camera(),
  _trackball()
{
}

Scene::~Scene()
{
  for(int i = 0, s = _objects.size(); i < s; ++i) {
    delete _objects.at(i);
  }
  for(int i = 0, s = _lights.size(); i < s; ++i) {
    delete _lights.at(i);
  }
}

void Scene::update(const InputState &state)
{
  for(int i = 0, s = _objects.size(); i < s; ++i) {
    _objects.at(i)->update(state);
  }
  _camera.move(state.mousePosition());
  _trackball.move(state.mousePosition());

  // TODO: add dirty flag (?)
  _view.setToIdentity();
  _view.lookAt(QVector3D(0.0, 0.0, 1.0),
               QVector3D(0.0, 0.0, 0.0),
               QVector3D(0.0, 1.0, 0.0));
  _view.translate(_camera.translation());
  _view.rotate(_trackball.rotation());
}

void Scene::draw(QGLShaderProgram *shaderProgram)
{
  for(int i = 0, s = _objects.size(); i < s; ++i) {
    Object *object = _objects.at(i);
    QGLShaderProgram *myShaderProgram;
    if(shaderProgram) {
      myShaderProgram = shaderProgram;
    }
    else {
      myShaderProgram = object->shaderProgram();
    }

    applyLights(myShaderProgram);

    object->draw(myShaderProgram, _projection, _view);
  }
}

void Scene::projection(float fov, float ratio, float near, float far)
{
  _projection.setToIdentity();
  _projection.perspective(fov, ratio, near, far);
}

void Scene::addObject(Object *object)
{
  _objects.append(object);
}

void Scene::addLight(Light *light)
{
  _lights.append(light);
}

void Scene::applyLights(QGLShaderProgram *shaderProgram)
{
  int count = _lights.size();
  for(int i = 0; i < count; ++i) {
    _lights.at(i)->apply(shaderProgram, i);
  }
}
