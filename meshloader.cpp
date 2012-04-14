#include "meshloader.h"

#include <fstream>

MeshData* MeshLoader::load(const char *filename)
{
  std::fstream stream(filename, std::fstream::in);
  return load(stream);
}


MeshData* MeshLoader::load(std::istream &input)
{
  return NULL;
}
