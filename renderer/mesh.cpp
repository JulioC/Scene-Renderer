#include "mesh.h"

#include <string.h>
#include <math.h>

#include <QVector3D>

MeshData::Attribute::Attribute(uint nVertices,
                               const char* identifier,
                               DataType dataType,
                               size_t dataSize,
                               uint nComponents) :
  _identifier(NULL),
  _dataType(dataType),
  _dataSize(dataSize),
  _nComponents(nComponents),
  _size(dataSize * nComponents),
  _data(NULL)
{
  // Make our own copy of the identifier
  _identifier = new char[strlen(identifier) + 1];
  strcpy(_identifier, identifier);

  // Allocate space for the components of every vertex
  _data = new char[_size * nVertices];
}

MeshData::Attribute::~Attribute()
{
  // Delete everything
  delete[] _identifier;
  delete[] _data;
}

void MeshData::Attribute::set(uint vertexId, const void *data)
{
  // We [hopefully] can do raw copying here
  memcpy(&(_data[vertexId * _size]), data, _size);
}

MeshData::MeshData(uint nVertices, uint nTriangles) :
  _nVertices(nVertices),
  _nTriangles(nTriangles),
  _vertices(NULL),
  _normals(NULL),
  _texCoords(NULL),
  _nAttributes(0),
  _attributes(),
  _triangles(NULL)
{
  // Allocate space for vertex data and triangles
  _vertices = new real_t[nVertices * 3];
  _triangles = new ushort[nTriangles * 3];
}

MeshData::~MeshData()
{
  // Delete everything
  for(uint i = 0; i < _nAttributes; ++i) {
    delete _attributes[i];
  }
  if(_texCoords) {
    delete[] _texCoords;
  }
  if(_normals) {
    delete[] _normals;
  }
  delete[] _vertices;
  delete[] _triangles;
}

int MeshData::regAttribute(const char* identifier,
                           DataType dataType,
                           size_t dataSize,
                           uint nComponents)
{
  // Check if we can add another attribute
  if(_nAttributes == MESH_MAX_ATTRIBUTES) {
    return -1;
  }

  // Create the Attribute with given parameters
  int index = _nAttributes;
  _attributes[index] = new Attribute(_nVertices, identifier, dataType, dataSize, nComponents);
  _nAttributes++;

  return index;
}

bool MeshData::setVertex(uint vertexId, real_t x, real_t y, real_t z)
{
  // Check if is valid vertex
  if(!(vertexId < _nVertices)) {
    return false;
  }

  // Copy the data in the proper segment
  int offset = vertexId * 3;
  _vertices[offset + 0] = x;
  _vertices[offset + 1] = y;
  _vertices[offset + 2] = z;

  return true;
}

bool MeshData::setAttribute(uint vertexId, uint attributeId, const void *data)
{
  // Check if is valid vertex and attribute
  if(!(vertexId < _nVertices) || !(attributeId < _nAttributes)) {
    return false;
  }

  // Attribute will handle the data copying
  _attributes[attributeId]->set(vertexId, data);

  return true;
}

bool MeshData::setTriangle(uint triangleId, ushort a, ushort b, ushort c)
{
  // Check if is valid triangle
  if(!(triangleId < _nTriangles)) {
    return false;
  }

  // Copy triangle elements
  int offset = triangleId * 3;
  _triangles[offset + 0] = a;
  _triangles[offset + 1] = b;
  _triangles[offset + 2] = c;

  return true;
}

void MeshData::normalize(real_t size)
{
  QVector3D vmin(0, 0, 0);
  QVector3D vmax(0, 0, 0);

  float min, max;
  min = max = 0.0;

  QVector3D *vertices = (QVector3D*)_vertices;

  // Get the min and max values from the vertex values
  for(uint i = 0; i < _nVertices; ++i) {
    if(vertices[i].x() < vmin.x()) { vmin.setX(vertices[i].x()); }
    if(vertices[i].y() < vmin.y()) { vmin.setY(vertices[i].y()); }
    if(vertices[i].z() < vmin.z()) { vmin.setZ(vertices[i].z()); }

    if(vertices[i].x() > vmax.x()) { vmax.setX(vertices[i].x()); }
    if(vertices[i].y() > vmax.y()) { vmax.setY(vertices[i].y()); }
    if(vertices[i].z() > vmax.z()) { vmax.setZ(vertices[i].z()); }

    if(vertices[i].length() < min) { min = vertices[i].length(); }
    if(vertices[i].length() > max) { max = vertices[i].length(); }
  }

  // Calcule the center and delta coef
  float deltainv = size / (max - min);
  QVector3D center =  (vmin + (vmax - vmin) / 2.0) * deltainv;

  // Apply the changes
  for(uint i = 0; i < _nVertices; ++i) {
    vertices[i] = vertices[i] * deltainv - center;
  }
}

void MeshData::computeNormals(bool weighted, const char *identifier)
{
  if(_normals) {
    return;
  }

  QVector3D *vertices = (QVector3D*)_vertices;

  // Get the sum of vertex normals
  _normals = new QVector3D[_nVertices];
  for(uint i = 0; i < _nTriangles; ++i) {
    int offset = i * 3;
    ushort elements[] = {
      _triangles[offset],
      _triangles[offset + 1],
      _triangles[offset + 2]
    };

    // Calculate the triangle (doubled) area
    QVector3D a = vertices[elements[0]] - vertices[elements[2]];
    QVector3D b = vertices[elements[1]] - vertices[elements[2]];
    QVector3D area = QVector3D::crossProduct(a, b);
    if(!weighted) {
      area.normalize();
    }

    // Store the normal to each vertex in triangle
    for(int j = 0; j < 3; ++j) {
      _normals[elements[j]] += area;
    }
  }

  // Register the normal attribute
  int attribute = regAttribute(identifier, Float, sizeof(float), 3);

  // Store the normals on the attribute
  for(uint i = 0; i < _nVertices; ++i) {
    _normals[i].normalize();
    setAttribute(i, attribute, &(_normals[i]));
  }
}

void MeshData::genTexCoords(TexCoordsMethod method, const char* identifier)
{
  if(_texCoords) {
    return;
  }

  _texCoords = new QVector2D[_nVertices];

  // Register the normal attribute
  int attribute = regAttribute(identifier, Float, sizeof(float), 2);

  switch(method) {
  case TexCoordsSphere:
    genTexCoordsSphere(attribute);
    break;
  case TexCoordsCylinder:
    genTexCoordsCylinder(attribute);
    break;
  }
}

void MeshData::genTexCoordsSphere(int attribute)
{
  if(!_normals) {
    return;
  }

  real_t u, v;
  for(uint i = 0; i < _nVertices; ++i)    {
      u = asin(_normals[i].x())/3.14 + 0.5;
      v = asin(_normals[i].y())/3.14 + 0.5;
      _texCoords[i] = QVector2D(u, v);
      setAttribute(i, attribute, &(_texCoords[i]));
  }
}

void MeshData::genTexCoordsCylinder(int attribute)
{
  QVector3D vmin(0, 0, 0);
  QVector3D vmax(0, 0, 0);

  QVector3D *vertices = (QVector3D*)_vertices;

  // Get the min and max values from the vertex values
  for(uint i = 0; i < _nVertices; ++i) {
    if(vertices[i].x() < vmin.x()) { vmin.setX(vertices[i].x()); }
    if(vertices[i].y() < vmin.y()) { vmin.setY(vertices[i].y()); }
    if(vertices[i].z() < vmin.z()) { vmin.setZ(vertices[i].z()); }

    if(vertices[i].x() > vmax.x()) { vmax.setX(vertices[i].x()); }
    if(vertices[i].y() > vmax.y()) { vmax.setY(vertices[i].y()); }
    if(vertices[i].z() > vmax.z()) { vmax.setZ(vertices[i].z()); }
  }

  QVector3D delta = (vmax - vmin);
  QVector3D deltainv(1.0/delta.x(), 1.0/delta.y(), 1.0/delta.z());

  real_t u, v, x;
  for(uint i = 0; i < _nVertices; ++i)    {
    x = 2.0 *(vertices[i].x() - vmin.x()) * deltainv.x() - 1.0;
      u = acos(x)/(3.14);
      v = (vertices[i].y() - vmin.y()) * deltainv.y();
      _texCoords[i] = QVector2D(u, v);
      setAttribute(i, attribute, &(_texCoords[i]));
  }
}

void MeshData::genTangents(const char *tIdentifier, const char *bIdentifier)
{
  if(!_texCoords || !_normals) {
    return;
  }

  QVector3D *tangents = new QVector3D[_nVertices];

  QVector3D *vertices = (QVector3D*)_vertices;

  for(uint i = 0; i < _nTriangles; ++i) {
    int offset = i * 3;
    ushort elements[] = {
      _triangles[offset],
      _triangles[offset + 1],
      _triangles[offset + 2]
    };

    QVector3D a = vertices[elements[1]] - vertices[elements[0]];
    QVector3D b = vertices[elements[2]] - vertices[elements[0]];

    QVector2D c = _texCoords[elements[1]] - _texCoords[elements[0]];
    QVector2D d = _texCoords[elements[2]] - _texCoords[elements[0]];

    float r = 1.0 / (c.x() * d.y() - d.x() * c.y());

    QVector3D sdir((d.y() * a.x() - c.y() * b.x()) * r,
                   (d.y() * a.y() - c.y() * b.y()) * r,
                   (d.y() * a.z() - c.y() * b.z()) * r);

    for(int j = 0; j < 3; ++j) {
      tangents[elements[j]] += sdir;
    }
  }

  int attrTangent = regAttribute(tIdentifier, Float, sizeof(float), 4);

  for(uint i = 0; i < _nVertices; ++i) {
    const QVector3D &n = _normals[i];
    const QVector3D &t = tangents[i];

    tangents[i] = (t - n * QVector3D::dotProduct(t, n)).normalized();

    setAttribute(i, attrTangent, &(tangents[i]));
  }

  delete[] tangents;
}

Mesh::Attribute::Attribute(const MeshData::Attribute &attribute, uint nVertices) :
  _identifier(NULL),
  _dataType(attribute.dataType()),
  _nComponents(attribute.nComponents()),
  _buffer(QGLBuffer::VertexBuffer)
{
  // Make our own copy of the identifier
  const char* identifier = attribute.identifier();
  _identifier = new char[strlen(identifier) + 1];
  strcpy(_identifier, identifier);

  // Build the buffer
  _buffer.create();
  _buffer.bind();
  _buffer.allocate(attribute.data(), attribute.dataSize() * _nComponents * nVertices);
}

Mesh::Attribute::~Attribute()
{
  // Clean up the buffer
  _buffer.destroy();

  // Delete the identifier
  delete[] _identifier;
}

void Mesh::Attribute::bind(QGLShaderProgram *shaderProgram)
{
  _buffer.bind();
  shaderProgram->enableAttributeArray(_identifier);
  shaderProgram->setAttributeBuffer(_identifier, _dataType, 0, _nComponents, 0);
}

Mesh::Mesh(const MeshData *meshData, const char* vIdentifier) :
  _vIdentifier(NULL),
  _nVertices(meshData->nVertices()),
  _nTriangles(meshData->nTriangles()),
  _nAttributes(meshData->nAttributes()),
  _attributes(NULL),
  _vertices(QGLBuffer::VertexBuffer),
  _triangles(QGLBuffer::IndexBuffer)
{
  // Create a local copy of the vertex identifier
  _vIdentifier = new char[strlen(vIdentifier) + 1];
  strcpy(_vIdentifier, vIdentifier);

  // Build the vertex buffer and copy the data
  _vertices.create();
  _vertices.bind();
  _vertices.allocate(meshData->vertices(), 3 * _nVertices * sizeof(float));

  // Build the index buffer and copy the data
  _triangles.create();
  _triangles.bind();
  _triangles.allocate(meshData->triangles(), 3 * _nTriangles * sizeof(ushort));

  // Create the attributes
  if(_nAttributes > 0) {
    _attributes = new Attribute*[_nAttributes];
    MeshData::Attribute * const* attributes = meshData->attributes();
    for(uint i = 0; i < _nAttributes; ++i) {
      _attributes[i] = new Attribute(*(attributes[i]), _nVertices);
    }
  }
}

Mesh::~Mesh()
{
  if(_attributes) {
    for(uint i = 0; i < _nAttributes; ++i) {
      delete _attributes[i];
    }
    delete[] _attributes;
  }

  // Clean up the buffers
  _triangles.destroy();
  _vertices.destroy();

}

void Mesh::draw(QGLShaderProgram *shaderProgram)
{
  // Bind the attributes
  for(uint i = 0; i < _nAttributes; ++i) {
    _attributes[i]->bind(shaderProgram);
  }

  // Bind the vertex position attribute
  _vertices.bind();
  shaderProgram->enableAttributeArray(_vIdentifier);
  shaderProgram->setAttributeBuffer(_vIdentifier, GL_FLOAT, 0, 3, 0);

  // Draw the elements
  _triangles.bind();
  glDrawElements(GL_TRIANGLES, _nTriangles * 3, GL_UNSIGNED_SHORT, 0);
}

