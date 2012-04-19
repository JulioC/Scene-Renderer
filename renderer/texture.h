#ifndef TEXTURE_H
#define TEXTURE_H

#include <QGLWidget>

class QGLShaderProgram;

// TODO: add generic Texture class

class Texture
{
public:
  Texture(const char* filename);
  ~Texture();

  void bind(QGLShaderProgram *shaderProgram, const char *identifier);

protected:
  GLuint _textureId;
  char *_identifier;

private:
  Texture(Texture &);
  Texture& operator=(Texture &);
};

#endif // TEXTURE_H
