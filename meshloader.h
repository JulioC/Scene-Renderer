#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <string>
#include <istream>

#include "mesh.h"

class MeshLoader
{
public:
  static MeshData* load(const char *filename);
  static MeshData* load(std::istream &input);

private:
  MeshLoader();
};

#endif // MESHLOADER_H
