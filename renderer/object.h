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

  void addTexture(Texture *texture);

  void scale(float scale);
  void position(const QVector3D &position);
  void rotation(float pitch, float yaw);

  virtual void update(const InputState &state, float delta);
  virtual void draw(QGLShaderProgram *shaderProgram, QMatrix4x4 &projectionMatrix, const QMatrix4x4 &viewMatrix);

  QGLShaderProgram* shaderProgram();

protected:
  float _scale;
  QVector3D _position;
  float _pitch, _yaw;

  QMatrix4x4 _modelMatrix;

  // TODO: should we really take care about this?
  QGLShaderProgram *_shaderProgram;

  Mesh *_mesh;
  Material *_material;
  QVector<Texture*> _textures;

  void rebuildModelMatrix();
};

#endif // OBJECT_H
