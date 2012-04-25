#include "object.h"

#include <QGLShaderProgram>

#include "material.h"
#include "mesh.h"
#include "texture.h"

Object::Object(Mesh *mesh, QGLShaderProgram *shaderProgram, Material *material):
  _scale(1.0),
  _position(0.0, 0.0, 0.0),
  _pitch(0.0),
  _yaw(0.0),
  _modelMatrix(),
  _shaderProgram(shaderProgram),
  _mesh(mesh),
  _material(material),
  _textures()
{
  rebuildModelMatrix();
}

Object::~Object()
{
  QList<QGLShader *> shaders = _shaderProgram->shaders();
  for(int i = 0, s = shaders.size(); i < s; ++i) {
    delete shaders.at(i);
  }
  delete _shaderProgram;

  delete _mesh;
  if(_material) {
    delete _material;
  }

  for(int i = 0, s = _textures.size(); i < s; ++i) {
    delete _textures.at(i);
  }
}

void Object::addTexture(Texture *texture)
{
  _textures.append(texture);
}

void Object::scale(float scale)
{
  _scale = scale;
  rebuildModelMatrix();
}

void Object::position(const QVector3D &position)
{
  _position = position;
  rebuildModelMatrix();
}

void Object::rotation(float pitch, float yaw)
{
  _pitch = pitch;
  _yaw = yaw;
  rebuildModelMatrix();
}

void Object::update(const InputState &state, float delta)
{
}

void Object::draw(QGLShaderProgram *shaderProgram, QMatrix4x4 &projectionMatrix, const QMatrix4x4 &viewMatrix)
{
  QMatrix4x4 mview, mvproj;

  mview = viewMatrix * _modelMatrix;
  mvproj = projectionMatrix * mview;

  shaderProgram->setUniformValue("ModelMatrix", _modelMatrix);
  shaderProgram->setUniformValue("ViewMatrix", viewMatrix);
  shaderProgram->setUniformValue("ProjectionMatrix", projectionMatrix);
  shaderProgram->setUniformValue("ModelViewMatrix", mview);
  shaderProgram->setUniformValue("ModelViewProjectionMatrix", mvproj);
  shaderProgram->setUniformValue("NormalMatrix", mview.normalMatrix());

  for(int i = 0, s = _textures.size(); i < s; ++i) {
    _textures.at(i)->bind(shaderProgram);
  }

  if(_material) {
    _material->apply(shaderProgram, "Material");
  }

  _mesh->draw(shaderProgram);
}

QGLShaderProgram* Object::shaderProgram()
{
  return _shaderProgram;
}

void Object::rebuildModelMatrix()
{
  _modelMatrix.setToIdentity();
  _modelMatrix.translate(_position);
  _modelMatrix.rotate(_pitch, 1.0, 0.0);
  _modelMatrix.rotate(_yaw, 0.0, 1.0);
  _modelMatrix.scale(_scale);
}

