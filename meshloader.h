#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <string>
#include <istream>

class MeshLoader
{
public:
  MeshLoader();
  virtual ~MeshLoader();

  virtual bool load(const char* filename, MeshData* meshData);
  virtual bool load(std::istream& input, MeshData* meshData) = 0;

};

#endif // MESHLOADER_H
