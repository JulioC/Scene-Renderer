#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>

class Camera
{
public:
  Camera(float moveRate = 1.0, float zoomRate = 0.001);

  void setAxis(const QVector3D& u, const QVector3D& v);

  void press(const QPointF& point);
  void release(const QPointF& point);

  void move(const QPointF& point);

  void zoom(int delta);

  void reset();

  inline QVector3D translation() const { return _translation; }

  inline float scale() const { return _scale; }

protected:
  float _moveRate;
  float _zoomRate;

  bool _pressed;

  QVector3D _u, _v;

  QPointF _lastPoint;
  QVector3D _translation;

  float _scale;
};

#endif // CAMERA_H
