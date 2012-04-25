#ifndef MESH_H
#define MESH_H

#include <stdlib.h>

#include <GL/gl.h>

#include <QGLBuffer>
#include <QGLShaderProgram>

#define MESH_MAX_ATTRIBUTES 16

typedef float real_t;
#define real_t_GL GL_FLOAT

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

  enum TexCoordsMethod
  {
    TexCoordsSphere,
    TexCoordsCylinder
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

  MeshData(uint nVertices, uint nTriangles);
  ~MeshData();

  inline uint nVertices() const { return _nVertices; }
  inline uint nTriangles() const { return _nTriangles; }

  inline const real_t* vertices() const { return _vertices; }

  inline uint nAttributes() const { return _nAttributes; }
  inline Attribute* const* attributes() const { return _attributes; }

  inline const ushort* triangles() const { return _triangles; }

  int regAttribute(const char* identifier,
                   DataType dataType,
                   size_t dataSize,
                   uint nComponents = 4);

  bool setVertex(uint vertexId, real_t x, real_t y, real_t z);
  bool setAttribute(uint vertexId, uint attributeId, const void* data);

  bool setTriangle(uint triangleId, ushort a, ushort b, ushort c);

  void normalize(real_t size = 1.0);
  void computeNormals(bool weighted = false, const char* identifier = "a_Normal");

  void genTexCoords(TexCoordsMethod method, const char* identifier = "a_TexCoords");

protected:
  uint _nVertices;
  uint _nTriangles;

  // TODO: use a type for everything
  real_t *_vertices;
  QVector3D *_normals;

  uint _nAttributes;
  Attribute *_attributes[MESH_MAX_ATTRIBUTES];

  ushort *_triangles;

  void genTexCoordsSphere(int attribute);
  void genTexCoordsCylinder(int attribute);

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
  Mesh(const MeshData *meshData, const char* vIdentifier = "a_Position");
  virtual ~Mesh();

  virtual void draw(QGLShaderProgram *shaderProgram);

protected:
  char* _vIdentifier;

  uint _nVertices;
  uint _nTriangles;

  uint _nAttributes;
  Attribute **_attributes;

  QGLBuffer _vertices;
  QGLBuffer _triangles;

private:
  Mesh();
  Mesh(Mesh &);
  Mesh& operator=(Mesh &);
};

#endif // MESH_H
