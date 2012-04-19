#include "camera.h"

Camera::Camera(float rate, InputState::MouseButton button):
  _button(button),
  _pressed(false),
  _rate(rate),
  _lastPoint(0.0, 0.0),
  _u(1.0, 0.0, 0.0),
  _v(0.0, 1.0, 0.0),
  _translation(0.0, 0.0, 0.0)
{
}

void Camera::setAxis(const QVector3D& u, const QVector3D& v)
{
  _u = u;
  _v = v;
}

void Camera::reset()
{
  _pressed = false;
  _translation = QVector3D(0.0, 0.0, 0.0);
}

bool Camera::update(const InputState &state, float delta)
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
  _translation += _rate * delta.x() * _u;
  _translation += _rate * delta.y() * _v;

  _lastPoint = point;
}
