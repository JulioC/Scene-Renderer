#ifndef KEYVALUES_H
#define KEYVALUES_H

#include <QVector3D>
#include <QVector4D>

class KeyValues
{
public:
  KeyValues();
  ~KeyValues();

  inline const char *name() const { return _name; }

  void setName(const char *name);
  void setValue(const char *value);

  bool loadFile(const char *filename);
  bool loadBuffer(char *buffer);

  KeyValues *find(const char *name);

  inline KeyValues *firstSubKey() { return _subKey; }
  inline KeyValues *nextKey() { return _nextKey; }
  void setNextKey(KeyValues *key);

  int getInt(const char *name = NULL, int value = 0);
  float getFloat(const char *name = NULL, float value = 0.0f);
  const char *getString(const char *name = NULL, const char *value = "");
  bool isEmpty(const char* name = NULL);

protected:
  KeyValues(const char *name);

  void loadBufferRecursive(char **buffer);

  char *readToken(const char **buffer, bool *quoted);

  char *_name;
  char *_value;

  KeyValues *_subKey;
  KeyValues *_nextKey;

private:
  KeyValues(KeyValues &);
  KeyValues& operator=(KeyValues &);
};

#endif // KEYVALUES_H
