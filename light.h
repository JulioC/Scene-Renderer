#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>
#include <QVector4D>

#include "material.h"

class QGLShaderProgram;

class Light
{
public:
  QVector3D position;
  QVector4D brightness;

  Light(const QVector3D &p, const QVector4D &b);
  ~Light();

  void apply(QGLShaderProgram *shaderProgram, const char *identifier);
};

#endif // LIGHT_H
