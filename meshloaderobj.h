#ifndef MESHLOADEROBJ_H
#define MESHLOADEROBJ_H

#include <meshloader.h>

class MeshLoaderOBJ : MeshLoader
{
public:
  MeshLoaderOBJ();
  virtual ~MeshLoaderOBJ();

  bool load(std::istream &input);

private:

};

#endif // MESHLOADEROBJ_H
