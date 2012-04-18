#include "light.h"

#include <string.h>
#include <stdio.h>

#include <QGLShaderProgram>

Light::Light(const QVector3D &position, const Material &material):
  _position(position),
  _material(material)
{
}

Light::~Light()
{
}

void Light::apply(QGLShaderProgram *shaderProgram, const char *identifier)
{
  char *buffer = new char[strlen(identifier) + 16];

  sprintf(buffer, "%s.position", identifier);
  shaderProgram->setUniformValue(identifier, _position);

  sprintf(buffer, "%s.material", identifier);
  _material.apply(shaderProgram, buffer);

  delete[] buffer;
}
