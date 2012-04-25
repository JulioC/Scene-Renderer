#include "texture.h"

#include <QGLWidget>
#include <QGLShaderProgram>

Texture::Texture(const char *filename, const char *identifier):
  _textureId(0),
  _identifier(NULL)
{
  _identifier = new char[strlen(identifier) + 1];
  strcpy(_identifier, identifier);

  // From Qt Boxes example

  glGenTextures(1, &_textureId);

  QImage image(filename);

  image = image.convertToFormat(QImage::Format_ARGB32);

  glBindTexture(GL_TEXTURE_2D, _textureId);

  glTexImage2D(GL_TEXTURE_2D, 0, 4, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
  glDeleteTextures(1, &_textureId);

  if(_identifier) {
    delete[] _identifier;
  }
}

void Texture::bind(QGLShaderProgram *shaderProgram)
{
  glActiveTexture(GL_TEXTURE0 + _textureId);
  glBindTexture(GL_TEXTURE_2D, _textureId);
  glEnable(GL_TEXTURE_2D);
  shaderProgram->setUniformValue(_identifier, _textureId);
}
