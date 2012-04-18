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

  bool getMouseButtonState(MouseButton button);
  void setMouseButtonState(MouseButton button, bool state);

  bool getModifierButtonState(ModifierButton modifier);
  void setModifierButtonState(ModifierButton button, bool state);

  QPointF getMousePosition();
  void setMousePosition(const QPointF &positions);

private:
  uint _mouseState;
  uint _modifierState;

  QPointF _mousePosition;
};

#endif // INPUTSTATE_H
