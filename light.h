#ifndef LIGHT_H
#define LIGHT_H

class QGLShaderProgram;

class Light
{
public:
  Light(insity, position);
  virtual Light();

  virtual apply(QGLShaderProgram *shaderProgram);
};

#endif // LIGHT_H
