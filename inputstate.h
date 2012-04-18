#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include <QPointF>

class InputState
{
public:
  enum MouseButton
  {
    MouseLeft   = 1 << 0,
    MouseMiddle = 1 << 1,
    MouseRight  = 1 << 2
  };
  enum ModifierButton
  {
    ModifierShift = 1 << 0,
    ModifierCtrl  = 1 << 1,
    ModifierAlt   = 1 << 2
  };

  InputState();

  void clearState();

  void update();

  bool modifier(ModifierButton button) const;
  void modifier(ModifierButton button, bool state);

  bool mouseButton(MouseButton button) const;
  void mouseButton(MouseButton button, bool state);

  bool mousePressed(MouseButton button) const;
  bool mouseDown(MouseButton button) const;
  bool mouseUp(MouseButton button) const;

  QPointF mousePosition() const;
  void mousePosition(const QPointF &position);

  QPointF mouseMotion() const;

private:
  uint _modifierState;

  uint _mouseState;
  uint _prevMouseState;

  QPointF _mousePosition;
  QPointF _prevMousePosition;

};

#endif // INPUTSTATE_H
