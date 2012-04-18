#ifndef OBJECT_H
#define OBJECT_H

#include <QMatrix4x4>

#include "inputstate.h"

class QGLShaderProgram;

class Mesh;

class Object
{
public:
  Object(Mesh *mesh, QGLShaderProgram *shaderProgram);
  virtual ~Object();

  virtual void update(const InputState &state);
  virtual void draw(QGLShaderProgram *shaderProgram, const QMatrix4x4 &projection, const QMatrix4x4 &view);

  QGLShaderProgram* shaderProgram();
  void shaderProgram(QGLShaderProgram *shaderProgram);

protected:
  // TODO: should we really take care about this?
  QGLShaderProgram *_shaderProgram;

  Mesh *_mesh;
};

#endif // OBJECT_H
