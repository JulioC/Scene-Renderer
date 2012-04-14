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
  _identifier = new char[strlen(identifier) + 1];
  strcpy(identifier, _identifier);

  // Allocate size for the components of every vertex
  _data = new char[_size * nVertices];
}

MeshData::Attribute::~Attribute()
{
  delete[] _identifier;
  delete[] _data;
}

void MeshData::Attribute::set(uint vertexId, const void *data)
{
  memcpy(&(_data[vertexId * _size]), data, _size);
}

MeshData::MeshData(const char *identifier, uint nVertices, uint nTriangles) :
  _identifier(NULL),
  _nVertices(nVertices),
  _nTriangles(nTriangles),
  _vPosition(NULL),
  _vNormal(NULL),
  _vAttributes(NULL),
  _triangles(NULL)
{
  _identifier = new char[strlen(identifier) + 1];
  strcpy(identifier, _identifier);

  _vPosition = new float[nVertices * 4];
  _vNormal = new float[nVertices * 4];

  for(int i = 0; i < MESH_MAX_ATTRIBUTES; ++i) {
    _vAttributes[i] = NULL;
  }

  _triangles = new uint[nTriangles * 3];
}

MeshData::~MeshData()
{
  for(int i = 0; i < MESH_MAX_ATTRIBUTES; ++i) {
    if(_vAttributes[i] != NULL) {
      delete _vAttributes[i];
    }
  }

  delete[] _identifier;
  delete[] _vPosition;
  delete[] _vNormal;
  delete[] _triangles;
}

int MeshData::regAttribute(const char* identifier,
                           DataType dataType,
                           size_t dataSize,
                           uint nComponents)
{
  int index;
  for(index = 0; index < MESH_MAX_ATTRIBUTES; ++index) {
    if(_vAttributes[index] == NULL) {
      break;
    }
  }

  if(index == MESH_MAX_ATTRIBUTES) {
    return -1;
  }

  _vAttributes[index] = new Attribute(_vertices, identifier, dataType, dataSize, nComponents);
  return index;
}

bool MeshData::setPosition(uint vertexId, float x, float y, float z, float w)
{
  if(vertexId >= _nVertices) {
    return false;
  }

  int offset = vertexId * 4;
  _vPosition[offset + 0] = x;
  _vPosition[offset + 1] = y;
  _vPosition[offset + 2] = z;
  _vPosition[offset + 3] = w;

  return true;
}

bool MeshData::setNormal(uint vertexId, float x, float y, float z, float w)
{
  if(vertexId >= _nVertices) {
    return false;
  }

  int offset = vertexId * 4;
  _vNormal[offset + 0] = x;
  _vNormal[offset + 1] = y;
  _vNormal[offset + 2] = z;
  _vNormal[offset + 3] = w;

  return true;
}

bool MeshData::setAttribute(uint vertexId, uint attributeId, const void *data)
{
  if(vertexId >= _nVertices || _vAttributes[attributeId] == NULL) {
    return false;
  }

  _vAttributes[attributeId]->set(vertexId, data);

  return true;
}
