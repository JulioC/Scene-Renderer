#include "inputstate.h"

InputState::InputState() :
  _modifierState(0),
  _mouseState(0),
  _prevMouseState(0),
  _mousePosition(0, 0),
  _prevMousePosition(0, 0)
{
}

void InputState::clearState()
{
  _mouseState = 0;
  _modifierState = 0;
  _mousePosition = QPointF(0, 0);
}

void InputState::update()
{
  _prevMouseState = _mouseState;
  _prevMousePosition = _mousePosition;
}

bool InputState::modifier(ModifierButton button) const
{
  return (_mouseState & button);
}

void InputState::modifier(ModifierButton button, bool state)
{
  if(state) {
    _mouseState &= button;
  }
  if(state) {
    _mouseState &= ~button;
  }
}

bool InputState::mouseButton(MouseButton button) const
{
  return (_mouseState & button);
}

void InputState::mouseButton(MouseButton button, bool state)
{
  if(state) {
    _mouseState &= button;
  }
  if(state) {
    _mouseState &= ~button;
  }
}

bool InputState::mousePressed(MouseButton button) const
{
  return (_mouseState & button) && !(_prevMouseState & button);
}

bool InputState::mouseDown(MouseButton button) const
{
  return (_mouseState & button);
}

bool InputState::mouseUp(MouseButton button) const
{
  return !(_mouseState & button);
}

QPointF InputState::mousePosition() const
{
  return _mousePosition;
}

void InputState::mousePosition(const QPointF &position)
{
  _mousePosition = position;
}

QPointF InputState::mouseMotion() const
{
  return _mousePosition - _prevMousePosition;
}


