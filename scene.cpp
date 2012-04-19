#include "scene.h"

#include <QGLShaderProgram>

#include "light.h"
#include "object.h"
#include "keyvalues.h"

Scene::Scene() :
  _lights(),
  _objects(),
  _projection(),
  _view(),
  _camera(),
  _trackball()
{
  projection(60.0, 1.0, 1.0, 100.0);

  // TODO: add method to set view
  _view.setToIdentity();
  _view.lookAt(QVector3D(0.0, 0.0, 1.0),
               QVector3D(0.0, 0.0, 0.0),
               QVector3D(0.0, 1.0, 0.0));
  _view.translate(QVector3D(0.0, 0.0, -1.0));
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

void Scene::update(const InputState &state, float delta)
{
  for(int i = 0, s = _objects.size(); i < s; ++i) {
    _objects.at(i)->update(state, delta);
  }

  bool dirty = false;
  if(_trackball.update(state, delta)) {
    dirty = true;
  }
  if(_camera.update(state, delta)) {
    dirty = true;
  }

  if(dirty) {
    _view.setToIdentity();
    _view.lookAt(QVector3D(0.0, 0.0, 1.0),
                 QVector3D(0.0, 0.0, 0.0),
                 QVector3D(0.0, 1.0, 0.0));
    _view.translate(QVector3D(0.0, 0.0, -1.0));
    _view.translate(_camera.translation());
    _view.rotate(_trackball.rotation());
  }
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
      myShaderProgram->bind();
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

  shaderProgram->setUniformValue("NumLights", count);

  char *buffer = new char[32];
  for(int i = 0; i < count; ++i) {
    sprintf(buffer, "Lights[%d]", i);
    _lights.at(i)->apply(shaderProgram, buffer);
  }
}
