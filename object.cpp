#include "object.h"

#include <QGLShaderProgram>

#include "mesh.h"

Object::Object(Mesh *mesh, QGLShaderProgram *shaderProgram):
  _shaderProgram(shaderProgram),
  _mesh(mesh)
{

}

Object::~Object()
{
  delete _shaderProgram;
  delete _mesh;
}

void Object::update(const InputState &state)
{
}

void Object::draw(QGLShaderProgram *shaderProgram, const QMatrix4x4 &projection, const QMatrix4x4 &view)
{
  // Bind matrices

  _mesh->draw(shaderProgram);
}

QGLShaderProgram* Object::shaderProgram()
{
  return _shaderProgram;
}

void Object::shaderProgram(QGLShaderProgram *shaderProgram)
{
  if(_shaderProgram) {
    delete _shaderProgram;
  }

  _shaderProgram = shaderProgram;
}


