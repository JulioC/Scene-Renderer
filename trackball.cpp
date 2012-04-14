#include "trackball.h"

#include <math.h>

Trackball::Trackball(float rate, float radius):
  _rotation(),
  _rate(rate),
  _radius(radius)
{
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

void Trackball::reset()
{
  _rotation = QQuaternion();
}

QQuaternion Trackball::rotation()
{
  return _rotation;
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
