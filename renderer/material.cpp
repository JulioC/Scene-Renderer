#include "material.h"

#include <stdio.h>
#include <string.h>

#include <QGLShaderProgram>

Material::Material(const QVector4D &a, const QVector4D &d, const QVector4D &s):
  ambient(a),
  diffuse(d),
  specular(s)
{
}

void Material::apply(QGLShaderProgram *shaderProgram, const char *identifier)
{
  char *buffer = new char[strlen(identifier) + 16];

  sprintf(buffer, "%s.ambient", identifier);
  shaderProgram->setUniformValue(buffer, ambient);

  sprintf(buffer, "%s.diffuse", identifier);
  shaderProgram->setUniformValue(buffer, diffuse);

  sprintf(buffer, "%s.specular", identifier);
  shaderProgram->setUniformValue(buffer, specular);

  delete[] buffer;
}
