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

  bool modifier(ModifierButton button);
  void modifier(ModifierButton button, bool state);

  bool mouseButton(MouseButton button);
  void mouseButton(MouseButton button, bool state);

  bool mouseButtonPressed(MouseButton button);
  bool mouseButtonDown(MouseButton button);
  bool mouseButtonUp(MouseButton button);

  QPointF mousePosition();
  void mousePosition(const QPointF &position);

  QPointF mouseMotion();

private:
  uint _modifierState;

  uint _mouseState;
  uint _prevMouseState;

  QPointF _mousePosition;
  QPointF _prevMousePosition;

};

#endif // INPUTSTATE_H
