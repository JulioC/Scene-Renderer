#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <QVector3D>
#include <QQuaternion>

class Trackball
{
public:
  enum Mode
  {
    None,
    Rotation
  };

  Trackball(float rate = 1.0, float radius = 1.0);

  void setDimensions(int width, int height);

  void press(const QPointF& point);
  void release(const QPointF& point);

  void move(const QPointF& point);

  void reset();

  QQuaternion rotation();

protected:
  bool _pressed;

  float _rate;
  float _radius;

  QVector3D _lastPosition;

  float _angle;
  QVector3D _axis;

  QQuaternion _rotation;

  QVector3D project(const QPointF& point);
};

#endif // TRACKBALL_H
