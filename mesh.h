#ifndef MESH_H
#define MESH_H

#include <stdlib.h>

#include <GL/gl.h>

#include <QGLBuffer>
#include <QGLShaderProgram>

#define MESH_MAX_ATTRIBUTES 16

class MeshData
{
public:
  enum DataType
  {
    Byte = GL_BYTE,
    UnsignedByte = GL_UNSIGNED_BYTE,
    Short = GL_SHORT,
    UnsignedShort = GL_UNSIGNED_SHORT,
    Int = GL_INT,
    UnsignedInt = GL_UNSIGNED_INT,
    Float = GL_FLOAT,
    Double = GL_DOUBLE
  };

  class Attribute
  {
  public:
    Attribute(uint nVertices,
              const char *identifier,
              DataType dataType,
              size_t dataSize,
              uint nComponents = 4);
    ~Attribute();

    inline const char* identifier() const { return _identifier; }
    inline DataType dataType() const { return _dataType; }
    inline size_t dataSize() const { return _dataSize; }
    inline uint nComponents() const { return _nComponents; }

    inline const void* data() const { return _data; }

    void set(uint vertexId, const void *data);

  protected:
    char *_identifier;
    DataType _dataType;
    size_t _dataSize;
    uint _nComponents;

    size_t _size;

    char* _data;

  private:
    Attribute(Attribute &);
    Attribute& operator=(Attribute &);
  };

public:
  MeshData(uint nVertices, uint nTriangles);
  ~MeshData();

  inline uint nVertices() const { return _nVertices; }
  inline uint nTriangles() const { return _nTriangles; }

  inline uint nAttributes() const { return _nAttributes; }
  inline Attribute* const* attributes() const { return _attributes; }

  inline const uint* triangles() const { return _triangles; }

  int regAttribute(const char* identifier,
                   DataType dataType,
                   size_t dataSize,
                   uint nComponents = 4);

  bool setPosition(uint vertexId, float x, float y, float z, float w = 1.0);
  bool setNormal(uint vertexId, float x, float y, float z, float w = 0.0);
  bool setAttribute(uint vertexId, uint attributeId, const void* data);

  void calcNormals();

protected:
  uint _nVertices;
  uint _nTriangles;

  float *_vPosition;
  float *_vNormal;

  uint _nAttributes;
  Attribute *_attributes[MESH_MAX_ATTRIBUTES];

  uint *_triangles;

private:
  MeshData(MeshData &);
  MeshData& operator=(MeshData &);
};

class Mesh {
protected:
  class Attribute {
  public:
    Attribute(const MeshData::Attribute &attribute, uint nVertices);
    ~Attribute();

    void bind(QGLShaderProgram *shaderProgram);

  protected:
    char *_identifier;
    GLenum _dataType;
    uint _nComponents;
    QGLBuffer _buffer;

  private:
    Attribute(Attribute &);
    Attribute& operator=(Attribute &);
  };

public:
  Mesh(const MeshData *meshData);
  ~Mesh();

  virtual void draw(QGLShaderProgram *shaderProgram);

protected:
  uint _nVertices;
  uint _nTriangles;

  uint _nAttributes;
  Attribute **_attributes;

  QGLBuffer _triangles;

private:
  Mesh(Mesh &);
  Mesh& operator=(Mesh &);
};

#endif // MESH_H
