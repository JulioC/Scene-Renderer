#include "keyvalues.h"

#include <string.h>

#define KEYVALUES_TOKEN_SIZE 1024

int KeyValues::counter = 0;

KeyValues::KeyValues():
  _name(NULL),
  _value(NULL),
  _subKey(NULL),
  _nextKey(NULL)
{
  counter++;
}

KeyValues::KeyValues(const char *name):
  _name(NULL),
  _value(NULL),
  _subKey(NULL),
  _nextKey(NULL)
{
  counter++;
  setName(name);
}

KeyValues::~KeyValues()
{
  if(_nextKey) {
    delete _nextKey;
  }

  if(_subKey) {
    delete _subKey;
  }

  if(_name) {
    delete[] _name;
  }
  if(_value) {
    delete[] _value;
  }
}

void KeyValues::setName(const char *name)
{
  if(_name) {
    delete[] _name;
  }

  _name = new char[strlen(name) + 1];
  strcpy(_name, name);
}

void KeyValues::setValue(const char *value)
{
  if(_value) {
    delete[] _value;
  }

  _value = new char[strlen(value) + 1];
  strcpy(_value, value);
}

bool KeyValues::loadFile(const char *filename)
{
  FILE *file;
  char *buffer;
  unsigned long length;

  file = fopen(filename, "r");
  if (!file) {
    return false;
  }

  // Get the file length
  fseek(file, 0, SEEK_END);
  length = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Create and read the buffer
  buffer = new char[length + 1];
  if (!buffer) {
    fclose(file);
    return false;
  }
  fread(buffer, length, 1, file);
  buffer[length] = '\0';

  fclose(file);

  bool status = loadBuffer(buffer);

  delete[] buffer;

  return status;
}

bool KeyValues::loadBuffer(char *buffer)
{
  bool quoted;
  KeyValues *current = this;
  KeyValues *previous = NULL;

  while(true) {
    const char *token;
    token = readToken(&buffer, &quoted);

    // End of file
    if(!buffer || !token || *token == '\0') {
      break;
    }

    if(!current) {
      // Creating the key
      current = new KeyValues(token);
      if(previous) {
        previous->setNextKey(current);
      }
    }
    else {
      current->setName(token);
    }

    // Inner block
    token = readToken(&buffer, &quoted);
    if(token && *token == '{' && !quoted) {
      current->loadBufferRecursive(&buffer);
    }

    previous = current;
    current = NULL;
    KeyValues(key);
  }

  return true;
}

void KeyValues::loadBufferRecursive(char **buffer)
{
  bool quoted;
  KeyValues *key = NULL;
  KeyValues *previous = NULL;

  while(true) {
    const char *token;

    token = readToken(buffer, &quoted);
    if(!token || *token == '\0') {
      break;
    }

    // End of block
    if(*token == '}' && !quoted) {
      break;
    }

    if(*token == '{' && !quoted) {
      if(key) {
        // Sub block
        key->loadBufferRecursive(buffer);
        previous = key;
        key = NULL;
      }
      else {
        break;
      }
    }
    else {
      if(!key) {
        // Creating the key
        key = new KeyValues(token);
        if(previous) {
          previous->setNextKey(key);
        }
        else {
          // The first sub key
          _subKey = key;
        }
      }
      else {
        key->setValue(token);
        previous = key;
        key = NULL;
      }
    }
  }
}

char *KeyValues::readToken(char **buffer, bool *quoted)
{
  static char buf[KEYVALUES_TOKEN_SIZE];

  if((*buffer) == NULL || (**buffer) == '\0') {
    return NULL;
  }

  char c = 0;
  int count = 0;

  *quoted = false;

  while(true) {
    // Ignore white spaces
    do {
      c = **buffer;
      (*buffer)++;
    } while((c > 0) && (c <= ' '));

    // End of file?
    if(!c) {
      return NULL;
    }

    // Ignore comments
    if(c == '#') {
      while((c > 0) && (c != '\n')) {
        c = **buffer;
        (*buffer)++;
      }
    }
    else {
      break;
    }
  }

  if(c == '\"') {
    *quoted = true;

    // Read a quoted value
    while (true) {
      c = **buffer;
      (*buffer)++;

      // Unexpected EOF
      if(c == 0) {
        return NULL;
      }

      // Ending quote
      if(c == '\"') {
        break;
      }

      // Control character
      if(c == '\\') {
        c = **buffer;
        (*buffer)++;

        switch(c) {
        case 0:    return NULL;
        case 'n':  c = '\n'; break;
        case 't':  c = '\t'; break;
        case '\\': c = '\\'; break;
        case '\"': c = '\\'; break;
        default:   c = c;
        }
      }

      if(count < (KEYVALUES_TOKEN_SIZE - 1)) {
        buf[count++] = c;
      }
    }
  }
  else if(c == '{' || c == '}'){
    buf[count++] = c;
  }
  else {
    return NULL;
  }

  if(count == 0) {
    return "";
  }

  buf[count] = '\0';

  return buf;
}

KeyValues *KeyValues::find(const char *name)
{
  for(KeyValues *cur = _subKey; cur; cur = cur->nextKey()) {
    if(strcmp(cur->name(), name) == 0) {
      return cur;
    }
  }

  return NULL;
}

void KeyValues::setNextKey(KeyValues *key)
{
  _nextKey = key;
}

int KeyValues::getInt(const char *name, int value)
{
  if(name) {
    KeyValues *key = find(name);
    return key ? key->getInt(NULL, value) : value;
  }

  return _value ? atoi(_value) : value;
}

float KeyValues::getFloat(const char *name, float value)
{
  if(name) {
    KeyValues *key = find(name);
    return key ? key->getFloat(NULL, value) : value;
  }

  return _value ? atof(_value) : value;
}

const char *KeyValues::getString(const char *name, const char *value)
{
  if(name) {
    KeyValues *key = find(name);
    return key ? key->getString(NULL, value) : value;
  }

  return _value ? _value : value;
}

bool KeyValues::isEmpty(const char* name)
{
  if(name) {
    KeyValues *key = find(name);
    return key ? key->isEmpty(NULL) : true;
  }

  return (_value == NULL);
}

void KeyValues::print(FILE *out, uint indentation)
{
  if(_name) {
    fprintf(out, "%*s\"%s\" ", indentation, " ", _name);
  }
  if(_value) {
    fprintf(out, "\"%s\" ", _value);
  }
  if(_subKey) {
    fprintf(out, "{\n");
    KeyValues *key = _subKey;
    while(key) {
      key->print(out, indentation + 2);
      key = key->nextKey();
    }
    fprintf(out, "%*s}", indentation, " ");
  }
  fprintf(out, "\n");
}

