#ifndef MESH_H
#define MESH_H

#include <stdlib.h>

class MeshData
{
public:
  enum DataType {
    Byte = GL_BYTE,
    UnsignedByte = GL_UNSIGNED_BYTE,
    Short = GL_SHORT,
    UnsignedShort = GL_UNSIGNED_SHORT,
    Int = GL_INT,
    UnsignedInt = GL_UNSIGNED_INT,
    Float = GL_FLOAT,
    Double = GL_DOUBLE
  };

protected:
  class Attribute
  {
  public:
    Attribute(uint vertices,
              const char *identifier,
              DataType dataType,
              size_t dataSize,
              uint components = 4);
    ~Attribute();

    inline const char* identifier() const { return _identifier; }
    inline DataType dataType() const { return _dataType; }
    inline size_t dataSize() const { return _dataSize; }
    inline uint components() const { return _components; }

    inline const void* data() const { return _data; };

    void set(uint vertexId, const void *data);

  protected:
    const char *_identifier;
    DataType _dataType;
    size_t _dataSize;
    uint _components;

    size_t _size;

    char* _data;
  };

public:
  MeshData(const char* identifier,
           uint vertices,
           uint triangles);
  ~MeshData();

  const char* identifier() const { return _identifier; }

  uint registerAttribute(const char* identifier,
                         DataType dataType,
                         size_t dataSize,
                         uint components = 4);

  bool setPosition(uint vertexId, float x, float y, float z, float w = 1.0);
  bool setNormal(uint vertexId, float x, float y, float z, float w = 0.0);
  bool setAttribute(uint vertexId, uint attribute, const void* data);

protected:
  float **_vPosition;
  float **_vNormal;
  Attribute *_vAttributes;
};

#endif // MESH_H
