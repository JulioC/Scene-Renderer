#ifndef LIGHT_H
#define LIGHT_H



class QGLShaderProgram;

class Light
{
public:
  Light();
  virtual ~Light();

  virtual void apply(QGLShaderProgram *shaderProgram, int id);
};

#endif // LIGHT_H
