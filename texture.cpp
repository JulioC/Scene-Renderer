#include "texture.h"

#include <QGLWidget>
#include <QGLShaderProgram>

Texture::Texture(const char *filename):
  _textureId(0),
  _identifier(NULL)
{
  // From Qt Boxes example

  glGenTextures(1, &_textureId);

  QImage image(filename);

  image = image.convertToFormat(QImage::Format_ARGB32);

  int width = image.width();
  int height = image.height();
  if (width != image.width() || height != image.height()) {
      image = image.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  }

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
}

void Texture::bind(QGLShaderProgram *shaderProgram, const char *identifier)
{
  glActiveTexture(_textureId);
  glBindTexture(GL_TEXTURE_2D, _textureId);
  glEnable(GL_TEXTURE_2D);
  shaderProgram->setUniformValue(identifier, _textureId);
}
