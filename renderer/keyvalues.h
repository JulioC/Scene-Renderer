#ifndef KEYVALUES_H
#define KEYVALUES_H

#include <stdlib.h>
#include <stdio.h>

class KeyValues
{
public:
  static int counter;

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
  const char *getString(const char *name = NULL, const char *value = NULL);
  bool isEmpty(const char* name = NULL);

  void print(FILE *out = stdout, uint indentation = 0);

protected:
  char *_name;
  char *_value;

  KeyValues *_subKey;
  KeyValues *_nextKey;

  KeyValues(const char *name);

  void loadBufferRecursive(char **buffer);

  char *readToken(char **buffer, bool *quoted);

private:
  KeyValues(KeyValues &);
  KeyValues& operator=(KeyValues &);
};

#endif // KEYVALUES_H
