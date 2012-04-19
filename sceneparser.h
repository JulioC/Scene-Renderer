#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include <QVector3D>
#include <QVector4D>

class QGLShader;

class Light;
class Object;
class Scene;
class KeyValues;

class SceneParser
{
public:
  static Scene *load(const char *filename);

protected:
  static Scene *loadScene(KeyValues *data);
  static Object *loadObject(KeyValues *data);
  static QGLShader *loadShader(KeyValues *data);
  static Light *loadLight(KeyValues *data);

  static QVector3D strToVector3D(const char *str);
  static QVector4D strToVector4D(const char *str);

  static char *currentPath;
  static void setPath(const char *filename);
  static char *resolvePath(const char *filename);
};

#endif // SCENEPARSER_H
