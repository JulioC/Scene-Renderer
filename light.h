#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

#include "material.h"

class QGLShaderProgram;

class Light
{
public:
  Light(const QVector3D &position, const Material &material);
  virtual ~Light();

  virtual void apply(QGLShaderProgram *shaderProgram, const char *identifier);

protected:
  QVector3D _position;
  Material _material;

};

#endif // LIGHT_H
