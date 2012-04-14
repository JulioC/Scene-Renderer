#include "meshloaderoff.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

MeshData* MeshLoaderOFF::load(const char *filename)
{
  std::fstream stream(filename, std::fstream::in);
  return load(stream);
}

MeshData* MeshLoaderOFF::load(std::istream &input)
{
  int lineNumber = 0;
  std::string line;

  // Magic word
  lineNumber++;
  char magic[3];
  input.read(magic, 3);
  input.ignore(1);
  if(!input) {
    std::cerr << "Read error at line " << lineNumber << std::endl;
    return NULL;
  }
  if((magic[0] != 'O') || (magic[1] != 'F') || (magic[2] != 'F')) {
    std::cerr << "Invalid magic word (found " << magic[0] << magic[1] << magic[2] << ") at line " << lineNumber << std::endl;
    return NULL;
  }

  // Get element counts
  lineNumber++;
  int nVertices, nFaces;
  if(!std::getline(input, line)) {
    std::cerr << "Parse error at line " << lineNumber << std::endl;
  }
  std::istringstream countstream(line);
  countstream >> nVertices >> std::ws >> nFaces;

  MeshData* meshData = new MeshData(nVertices, nFaces);

  // Parse the vertexes
  for(int i = 0; i < nVertices; ++i) {
    lineNumber++;
    if(!std::getline(input, line)) {
      std::cerr << "Read error at line " << lineNumber << std::endl;
      delete meshData;
      return NULL;
    }

    real_t x, y, z;
    std::istringstream linestream(line);
    linestream >> x >> std::ws >> y >> std::ws >> z;

    meshData->setVertex(i, x, y, z);
  }

  // Parse the faces
  for(int i = 0; i < nFaces; ++i) {
    lineNumber++;
    if(!std::getline(input, line)) {
      std::cerr << "Read error at line " << lineNumber << std::endl;
      delete meshData;
      return NULL;
    }

    uint count, a, b, c;
    std::istringstream linestream(line);
    linestream >> count >> std::ws >> a >> std::ws >> b >> std::ws >> c;
    if(count != 3) {
      std::cerr << "Invalid face at line " << lineNumber << std::endl;
      delete meshData;
      return NULL;
    }

    meshData->setTriangle(i, a, b, c);
  }

  return meshData;
}
