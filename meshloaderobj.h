#ifndef MESHLOADEROBJ_H
#define MESHLOADEROBJ_H

#include <meshloader.h>

class MeshLoaderOBJ : MeshLoader
{
public:
  static MeshData* load(std::istream &input);
};

#endif // MESHLOADEROBJ_H
