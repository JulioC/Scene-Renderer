#ifndef MESHLOADEROFF_H
#define MESHLOADEROFF_H

#include <meshloader.h>

class MeshLoaderOFF : MeshLoader
{
public:
  static MeshData* load(std::istream &input);

private:
  MeshLoaderOFF();
};

#endif // MESHLOADEROFF_H
