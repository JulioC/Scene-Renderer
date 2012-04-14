#include "meshloader.h"

#include <fstream>

MeshLoader::MeshLoader() :
  _loaded(false)
{
}

MeshLoader::~MeshLoader()
{
}

bool MeshLoader::load(const char* filename, MeshData* meshData)
{
  std::fstream stream(filename, std::fstream::in);
  return load(stream, meshData);
}
