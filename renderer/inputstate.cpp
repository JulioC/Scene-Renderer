#include "inputstate.h"

InputState::InputState() :
  _nextMouseState(MouseNone),
  _mouseState(MouseNone),
  _prevMouseState(MouseNone),
  _nextMousePosition(0, 0),
  _mousePosition(0, 0),
  _prevMousePosition(0, 0)
{
}

void InputState::clearState()
{
  _nextMouseState = MouseNone;
  _mouseState = MouseNone;
  _prevMouseState = MouseNone;
  _mousePosition = QPointF(0, 0);
}

void InputState::update()
{
  _prevMouseState = _mouseState;
  _mouseState = _nextMouseState;

  _prevMousePosition = _mousePosition;
  _mousePosition = _nextMousePosition;
}

bool InputState::mouseButton(MouseButton button) const
{
  return button && (_mouseState & button);
}

void InputState::mouseButton(MouseButton button, bool state)
{
  if(state) {
    _nextMouseState |= button;
  }
  else {
    _nextMouseState &= ~button;
  }
}

bool InputState::mousePressed(MouseButton button) const
{
  return button && (_mouseState & button) && !(_prevMouseState & button);
}

bool InputState::mouseDown(MouseButton button) const
{
  return button && (_mouseState & button);
}

bool InputState::mouseUp(MouseButton button) const
{
  return button && !(_mouseState & button);
}

QPointF InputState::mousePosition() const
{
  return _mousePosition;
}

void InputState::mousePosition(const QPointF &position)
{
  _nextMousePosition = position;
}

QPointF InputState::mouseMotion() const
{
  return _mousePosition - _prevMousePosition;
}


