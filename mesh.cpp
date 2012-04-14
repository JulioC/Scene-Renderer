#include "mesh.h"

#include <string.h>

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
  _vPosition(NULL),
  _vNormal(NULL),
  _nAttributes(0),
  _attributes(),
  _triangles(NULL)
{
  // Allocate space for vertex data and triangles
  _vPosition = new float[nVertices * 4];
  _vNormal = new float[nVertices * 4];
  _triangles = new uint[nTriangles * 3];
}

MeshData::~MeshData()
{
  // Delete everything
  for(uint i = 0; i < _nAttributes; ++i) {
    delete _attributes[i];
  }
  delete[] _vPosition;
  delete[] _vNormal;
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

bool MeshData::setPosition(uint vertexId, float x, float y, float z, float w)
{
  // Check if is valid vertex
  if(!(vertexId < _nVertices)) {
    return false;
  }

  // Copy the data in the proper segment
  int offset = vertexId * 4;
  _vPosition[offset + 0] = x;
  _vPosition[offset + 1] = y;
  _vPosition[offset + 2] = z;
  _vPosition[offset + 3] = w;

  return true;
}

bool MeshData::setNormal(uint vertexId, float x, float y, float z, float w)
{
  // Check if is valid vertex
  if(!(vertexId < _nVertices)) {
    return false;
  }

  // Copy the data in the proper segment
  int offset = vertexId * 4;
  _vNormal[offset + 0] = x;
  _vNormal[offset + 1] = y;
  _vNormal[offset + 2] = z;
  _vNormal[offset + 3] = w;

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

Mesh::Mesh(const MeshData *meshData) :
  _nVertices(meshData->nVertices()),
  _nTriangles(meshData->nVertices()),
  _nAttributes(meshData->nAttributes()),
  _attributes(NULL),
  _triangles(QGLBuffer::IndexBuffer)
{
  // Build the index buffer and copy the data
  _triangles.create();
  _triangles.bind();
  _triangles.allocate(meshData->triangles(), 3 * sizeof(ushort));

  // Create the attributes
  _attributes = new Attribute*[_nAttributes];
  MeshData::Attribute * const* attributes = meshData->attributes();
  for(int i = 0; i < _nAttributes; ++i) {
    _attributes[i] = new Attribute(*(attributes[i]), _nVertices);
  }
}

void Mesh::draw(QGLShaderProgram *shaderProgram)
{
  // Bind the attributes
  for(int i = 0; i < _nAttributes; ++i) {
    _attributes[i]->bind(shaderProgram);
  }

  // Draw the elements
  _triangles.bind();
  glDrawElements(GL_TRIANGLES, _nTriangles, GL_UNSIGNED_SHORT, 0);
}

