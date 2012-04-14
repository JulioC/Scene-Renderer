#include "mesh.h"

#include <string.h>

MeshData::Attribute::Attribute(uint vertices,
                               const char* identifier,
                               DataType dataType,
                               size_t dataSize,
                               uint components = 4) :
  _identifier(NULL),
  _dataType(dataType),
  _dataSize(dataSize),
  _components(components),
  _size(dataSize * components),
  _data(NULL)
{
  _identifier = new char[strlen(identifier) + 1];
  strcpy(identifier, _identifier);

  // Allocate size for the components of every vertex
  _data = new char[_size * vertices];
}

MeshData::Attribute::~Attribute() {
  delete _identifier;
  delete _data;
}

void MeshData::Attribute::set(uint vertexId, const void *data) {
  memcpy(&(_data[vertexId * _size]), data, _size);
}

