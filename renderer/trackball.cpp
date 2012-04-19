#include "trackball.h"

#include <math.h>

Trackball::Trackball(float rate, float radius, InputState::MouseButton button):
  _button(button),
  _pressed(false),
  _rate(rate),
  _radius(radius),
  _lastPosition(0.0, 0.0, 0.0),
  _angle(0.0),
  _axis(0.0, 0.0, 0.0),
  _rotation(1.0, 0.0, 0.0, 0.0)
{
}

void Trackball::reset()
{
  _pressed = false;
  _rotation = QQuaternion();
}

bool Trackball::update(const InputState &state, float delta)
{
  bool changed = false;
  if(_pressed) {
    if(!state.mouseButton(_button)) {
      release(state.mousePosition());
    }
    move(state.mousePosition());
    changed = true;
  }
  else {
    if(state.mousePressed(_button)) {
      press(state.mousePosition());
      changed = true;
    }
  }
  return changed;
}

void Trackball::press(const QPointF &point)
{
  QPointF p = point;
  p *= 1.0 / _radius;
  _lastPosition = project(p);
  _pressed = true;
}

void Trackball::release(const QPointF &point)
{
  // TODO: Keep acceleration
  _pressed = false;
}

void Trackball::move(const QPointF &point)
{
  if(!_pressed) {
    return;
  }

  QPointF p = point;
  p *= 1.0 / _radius;

  QVector3D position = project(p);

  QVector3D deltap = position - _lastPosition;
  if(deltap.length() > 0.0) {
    _axis = QVector3D::crossProduct(_lastPosition, position);
    _axis.normalize();
    _angle = _rate * 180 / M_PI * acos(QVector3D::dotProduct(_lastPosition, position));

    _rotation = QQuaternion::fromAxisAndAngle(_axis, _angle) * _rotation;
    _rotation.normalize();
  }

  _lastPosition = position;
}

QVector3D Trackball::project(const QPointF &point)
{
  float d, x, y, z;

  x = fabs(point.x()) > 1.0 ? (point.x() / fabs(point.x())) : point.x();
  y = fabs(point.y()) > 1.0 ? (point.y() / fabs(point.y())) : point.y();

  d = sqrt(x*x + y*y);
  z = cos(M_PI_2 * (d < 1.0 ? d : 1.0));

  return QVector3D(x, y, z).normalized();
}
