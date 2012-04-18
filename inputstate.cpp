#include "inputstate.h"

InputState::InputState() :
  _mouseState(0),
  _modifierState(0),
  _mousePosition(0, 0)
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

bool InputState::modifier(ModifierButton button)
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

bool InputState::mouseButton(MouseButton button)
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

bool InputState::mouseButtonPressed(MouseButton button)
{
  return (_mouseState & button) && !(_prevMouseState & button);
}

bool InputState::mouseButtonDown(MouseButton button)
{
  return (_mouseState & button);
}

bool InputState::mouseButtonUp(MouseButton button)
{
  return !(_mouseState & button);
}

QPointF InputState::mousePosition()
{
  return _mousePosition;
}

void InputState::mousePosition(const QPointF &position)
{
  _mousePosition = position;
}

QPointF InputState::mouseMotion()
{
  return _mousePosition - _prevMousePosition;
}


