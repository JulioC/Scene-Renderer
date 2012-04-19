#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <QVector3D>
#include <QQuaternion>

#include "inputstate.h"

class Trackball
{
public:
  enum Mode
  {
    None,
    Rotation
  };

  Trackball(float rate = 1.0, float radius = 1.0, InputState::MouseButton button = InputState::MouseLeft);

  void setDimensions(int width, int height);

  void update(const InputState &state, float delta);

  void reset();

  QQuaternion rotation();

protected:
  InputState::MouseButton _button;
  bool _pressed;

  float _rate;
  float _radius;

  QVector3D _lastPosition;

  float _angle;
  QVector3D _axis;

  QQuaternion _rotation;

  void press(const QPointF& point);
  void release(const QPointF& point);

  void move(const QPointF& point);

  QVector3D project(const QPointF& point);
};

#endif // TRACKBALL_H
