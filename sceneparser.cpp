#include "sceneparser.h"

#include "keyvalues.h"
#include "light.h"
#include "mesh.h"
#include "meshloaderoff.h"
#include "meshloaderply.h"
#include "object.h"
#include "scene.h"

Scene *SceneParser::load(const char *filename)
{
  KeyValues *data = new KeyValues();
  if(!data->loadFile(filename)) {
    fprintf(stderr, "Failed to load Scene");
    return NULL;
  }

  // TODO: should we have an internal object for the parser?
  setPath(filename);

  return loadScene(data);
}

Scene *SceneParser::loadScene(KeyValues *data)
{
  Scene *scene = new Scene();

  KeyValues *key = data->firstSubKey();
  while(key) {
    const char *name = key->name();

    if(strcmp(name, "object") == 0) {
      Object *object = loadObject(key);
      if(object) {
        scene->addObject(object);
      }
      else {
        fprintf(stderr, "Failed to load Object");
      }
    }
    else if(strcmp(name, "light") == 0) {
      Light *light = loadLight(key);
      if(light) {
        scene->addLight(light);
      }
      else {
        fprintf(stderr, "Failed to load Light");
      }
    }
    else {
      fprintf(stderr, "Unknown element: %s", name);
    }

    key = key->nextKey();
  }

  return scene;
}

Object *SceneParser::loadObject(KeyValues *data)
{
  KeyValues *key;

  key = data->find("format");
  if(!key) {
    return NULL;
  }
  const char *format = key->getString();

  Mesh *mesh;
  if(strcmp("OFF", format) == 0) {
    key = data->find("file");
    if(!key) {
      return NULL;
    }
    const char *file = key->getString();
    char *filename = resolvePath(file);

    MeshData *meshData = MeshLoaderOFF::load(filename);
    if(!meshData) {
      fprintf(stderr, "Failed to load MeshData of %s", filename);
      delete[] filename;
      return NULL;
    }

    meshData->normalize();
    meshData->computeNormals();

    mesh = new Mesh(meshData);

    delete meshData;
    delete[] filename;
  }
  else if(strcmp("PLY", format) == 0) {
    key = data->find("file");
    if(!key) {
      return NULL;
    }
    const char *file = key->getString();
    char *filename = resolvePath(file);

    MeshData *meshData = MeshLoaderPLY::load(filename);
    if(!meshData) {
      fprintf(stderr, "Failed to load MeshData of %s", filename);
      delete[] filename;
      return NULL;
    }

    meshData->normalize();
    meshData->computeNormals();

    mesh = new Mesh(meshData);

    delete meshData;
    delete[] filename;
  }
  else {
    fprintf(stderr, "Invalid object format: %s", format);
    return NULL;
  }

  QGLShaderProgram *shaderProgram = new QGLShaderProgram();

  key = data->firstSubKey();
  while(key) {
    if(strcmp(key->name(), "shader") == 0) {
      QGLShader *shader = loadShader(key);
      if(shader) {
        shaderProgram->addShader(shader);
      }
      else {
        fprintf(stderr, "Failed to load shader");
      }
    }
    else if (strcmp(key->name(), "material") == 0) {
      fprintf(stderr, "Loading material");
      key->print(stderr);
    }
    else if (strcmp(key->name(), "texture") == 0) {
      fprintf(stderr, "Loading texture");
      key->print(stderr);
    }
    key = key->nextKey();
  }

  if(!shaderProgram->link()) {
    fprintf(stderr, "Failed to link shader program");
  }

  return new Object(mesh, shaderProgram);
}

QGLShader *SceneParser::loadShader(KeyValues *data)
{
  KeyValues *key;

  key = data->find("type");
  if(!key) {
    return NULL;
  }
  const char *type = key->getString();

  key = data->find("file");
  if(!key) {
    return NULL;
  }
  const char *file = key->getString();
  char *filename = resolvePath(file);

  QGLShader::ShaderType shaderType;
  if(strcmp(type, "vertex") == 0) {
    shaderType = QGLShader::Vertex;
  }
  else if(strcmp(type, "fragment") == 0) {
    shaderType = QGLShader::Fragment;
  }
  else {
    fprintf(stderr, "Invalid shader type: %s", type);
    delete[] filename;
    return NULL;
  }

  QGLShader *shader = new QGLShader(shaderType);
  if(!shader->compileSourceFile(filename)) {
    fprintf(stderr, "Failed to load shader %s", filename);
    fprintf(stderr, shader->log().toStdString().c_str());
    delete shader;
    delete[] filename;
    return NULL;
  }

  return shader;
}

Light *SceneParser::loadLight(KeyValues *data)
{
  KeyValues *key = NULL;

  key = data->find("position");
  if(!key) {
    fprintf(stderr, "No position find for Light");
    return NULL;
  }
  QVector3D position = strToVector3D(key->getString());

  key = data->find("brightness");
  if(!key) {
    fprintf(stderr, "No brightness find for Light");
    return NULL;
  }
  QVector4D brightness = strToVector4D(key->getString());

  return new Light(position, brightness);
}

QVector3D SceneParser::strToVector3D(const char *str)
{
  float x, y, z;
  sscanf(str, "%f %f %f", &x, &y, &z);
  return QVector3D(x, y, z);
}

QVector4D SceneParser::strToVector4D(const char *str)
{
  float x, y, z, w;
  sscanf(str, "%f %f %f %f", &x, &y, &z, &w);
  return QVector4D(x, y, z, w);
}

char *SceneParser::currentPath = NULL;

void SceneParser::setPath(const char *filename)
{
  int len = strlen(filename);
  while(filename[len] != '/') {
    len--;
  }

  currentPath = new char[len + 1];
  for(int i = 0; i <= len; ++i) {
    currentPath[i] = filename[i];
  }
  currentPath[len + 1] = '\0';
}

char *SceneParser::resolvePath(const char *filename)
{
  bool relative = (filename[0] != '/' && filename[0] != ':');
  int len = strlen(filename);
  if(relative) {
    len += strlen(currentPath);
  }

  int offset = 0;
  char *ret = new char[len + 1];
  if(relative) {
    for(char *c = currentPath; *c; c++) {
      ret[offset++] = *c;
    }
  }
  strcpy(&(ret[offset]), filename);
  return ret;
}

