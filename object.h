#ifndef OBJECT_H
#define OBJECT_H

#include <QPair>
#include <QVector>
#include <QMatrix4x4>

#include "inputstate.h"

class QGLShaderProgram;

class Material;
class Mesh;
class Texture;

class Object
{
public:
  Object(Mesh *mesh, QGLShaderProgram *shaderProgram, Material *material = NULL);
  virtual ~Object();

  void addTexture(Texture *texture, const char *identifier);

  virtual void update(const InputState &state);
  virtual void draw(QGLShaderProgram *shaderProgram, const QMatrix4x4 &projection, const QMatrix4x4 &view);

  QGLShaderProgram* shaderProgram();

protected:
  typedef QPair<const char*, Texture*> TexPair;

  // TODO: should we really take care about this?
  QGLShaderProgram *_shaderProgram;

  Mesh *_mesh;
  Material *_material;
  QVector<TexPair> _textures;
};

#endif // OBJECT_H