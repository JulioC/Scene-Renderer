#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include <QPointF>

class InputState
{
public:
  enum MouseButton
  {
    MouseNone   = 0,
    MouseLeft   = 1 << 0,
    MouseMiddle = 1 << 1,
    MouseRight  = 1 << 2
  };

  InputState();

  void clearState();

  void update();

  bool mouseButton(MouseButton button) const;
  void mouseButton(MouseButton button, bool state);

  bool mousePressed(MouseButton button) const;
  bool mouseDown(MouseButton button) const;
  bool mouseUp(MouseButton button) const;

  QPointF mousePosition() const;
  void mousePosition(const QPointF &position);

  QPointF mouseMotion() const;

private:
  uint _nextMouseState;
  uint _mouseState;
  uint _prevMouseState;

  QPointF _nextMousePosition;
  QPointF _mousePosition;
  QPointF _prevMousePosition;

};

#endif // INPUTSTATE_H
