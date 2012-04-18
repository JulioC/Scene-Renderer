#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector4D>

class QGLShaderProgram;

class Material
{
public:
  Material(const QVector4D &a, const QVector4D &d, const QVector4D &s);

  virtual void apply(QGLShaderProgram *shaderProgram, const char *identifier);

  QVector4D ambient;
  QVector4D diffuse;
  QVector4D specular;
};

#endif // MATERIAL_H
