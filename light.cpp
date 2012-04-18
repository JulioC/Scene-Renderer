#include "light.h"

#include <string.h>
#include <stdio.h>

#include <QGLShaderProgram>

Light::Light(const QVector3D &p, const QVector4D &b):
  position(p),
  brightness(b)
{
}

Light::~Light()
{
}

void Light::apply(QGLShaderProgram *shaderProgram, const char *identifier)
{
  char *buffer = new char[strlen(identifier) + 16];

  sprintf(buffer, "%s.position", identifier);
  shaderProgram->setUniformValue(identifier, position);

  sprintf(buffer, "%s.brightness", identifier);
  shaderProgram->setUniformValue(identifier, brightness);

  delete[] buffer;
}
