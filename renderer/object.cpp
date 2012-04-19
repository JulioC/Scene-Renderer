#include "object.h"

#include <QGLShaderProgram>

#include "material.h"
#include "mesh.h"
#include "texture.h"

Object::Object(Mesh *mesh, QGLShaderProgram *shaderProgram, Material *material):
  _shaderProgram(shaderProgram),
  _mesh(mesh),
  _material(material),
  _textures()
{
}

Object::~Object()
{
  QList<QGLShader *> shaders = _shaderProgram->shaders();
  for(int i = 0, s = shaders.size(); i < s; ++i) {
    delete shaders.at(i);
  }
  delete _shaderProgram;

  delete _mesh;
  if(_material) {
    delete _material;
  }

  for(int i = 0, s = _textures.size(); i < s; ++i) {
    TexPair pair = _textures.at(i);
    delete[] pair.first;
    delete pair.second;
  }
}

void Object::addTexture(Texture *texture, const char *identifier)
{
  char *buffer = new char[strlen(identifier) + 1];
  strcpy(buffer, identifier);
  _textures.append(TexPair(buffer, texture));
}

void Object::update(const InputState &state, float delta)
{
}

void Object::draw(QGLShaderProgram *shaderProgram, const QMatrix4x4 &projection, const QMatrix4x4 &view)
{
  QMatrix4x4 model, mview, mvproj;

  model.setToIdentity();
  mview = view * model;
  mvproj = projection * mview;

  shaderProgram->setUniformValue("ModelMatrix", model);
  shaderProgram->setUniformValue("ViewMatrix", view);
  shaderProgram->setUniformValue("ProjectionMatrix", projection);
  shaderProgram->setUniformValue("ModelViewMatrix", mview);
  shaderProgram->setUniformValue("ModelViewProjectionMatrix", mvproj);
  shaderProgram->setUniformValue("NormalMatrix", mview.normalMatrix());

  for(int i = 0, s = _textures.size(); i < s; ++i) {
    TexPair pair = _textures.at(i);
    pair.second->bind(shaderProgram, pair.first);
  }

  if(_material) {
    _material->apply(shaderProgram, "Material");
  }

  _mesh->draw(shaderProgram);
}

QGLShaderProgram* Object::shaderProgram()
{
  return _shaderProgram;
}


