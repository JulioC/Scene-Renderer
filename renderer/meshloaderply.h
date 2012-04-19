#ifndef MESHLOADERPLY_H
#define MESHLOADERPLY_H

#include <istream>

class MeshData;

class MeshLoaderPLY
{
public:
  static MeshData* load(const char *filename);
  static MeshData* load(std::istream &input);

private:
  MeshLoaderPLY();
};

#endif // MESHLOADERPLY_H
