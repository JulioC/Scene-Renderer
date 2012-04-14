#ifndef MESHLOADEROFF_H
#define MESHLOADEROFF_H

#include <istream>

class MeshData;

class MeshLoaderOFF
{
public:
  static MeshData* load(const char *filename);
  static MeshData* load(std::istream &input);

private:
  MeshLoaderOFF();
};

#endif // MESHLOADEROFF_H
