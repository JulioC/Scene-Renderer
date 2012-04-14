#include "camera.h"

Camera::Camera(float moveRate, float zoomRate):
  _moveRate(moveRate),
  _zoomRate(zoomRate),
  _pressed(false),
  _u(1.0, 0.0, 0.0),
  _v(0.0, 1.0, 0.0),
  _lastPoint(0.0, 0.0),
  _translation(0.0, 0.0, 0.0),
  _scale(1.0)
{
}

void Camera::setAxis(const QVector3D& u, const QVector3D& v)
{
  _u = u;
  _v = v;
}

void Camera::press(const QPointF& point)
{
  _pressed = true;
  _lastPoint = point;
}

void Camera::release(const QPointF& point)
{
  _pressed = false;
}

void Camera::move(const QPointF& point)
{
  if(!_pressed) {
    return;
  }

  QPointF delta = point - _lastPoint;
  _translation += _moveRate * delta.x() * _u;
  _translation += _moveRate * delta.y() * _v;

  _lastPoint = point;
}

void Camera::zoom(int delta)
{
  _scale += _zoomRate * delta;
}

void Camera::reset()
{
  _translation = QVector3D(0.0, 0.0, 0.0);
  _scale = 1.0;
}
