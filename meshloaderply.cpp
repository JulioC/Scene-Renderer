#include "meshloaderply.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "mesh.h"

MeshData* MeshLoaderPLY::load(const char *filename)
{
  std::fstream stream(filename, std::fstream::in);
  return load(stream);
}

MeshData* MeshLoaderPLY::load(std::istream &input)
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
  if((magic[0] != 'p') || (magic[1] != 'l') || (magic[2] != 'y')) {
    std::cerr << "Invalid magic word (" << magic[0] << magic[1] << magic[2] << ") at line " << lineNumber << std::endl;
    return NULL;
  }

  int nVertices, nFaces;

  // Parsing the header
  while(std::getline(input, line)) {
    ++lineNumber;
    std::istringstream linestream(line);

    std::string keyword;
    linestream >> keyword;

    if(keyword == "format") {
      std::string format, version;
      linestream >> format >> version;
      // TODO: implement binary format reading
      if (format != "ascii") {
        std::cerr << "Unsupported format (" << format <<  ") at line " << lineNumber << std::endl;
        return NULL;
      }
      // There are no other known versions
      if (version != "1.0") {
        std::cerr << "Unsupported version (" << version <<  ") at line " << lineNumber << std::endl;
        return NULL;
      }
    }
    else if(keyword == "element") {
      std::string element;
      int count;
      linestream >> element >> count;

      // TODO: we should be able to read any element

      if(element == "vertex") {
        nVertices = count;
      }
      else if(element == "face") {
        nFaces = count;
      }
      else {
        std::cerr << "Unsupported element (" << element <<  ") at line " << lineNumber << std::endl;
        return NULL;
      }
    }
    else if(keyword == "property") {
      // TODO: we should read the element properties and store instead of hardcoding them
    }
    else if(keyword == "comment") {
      std::cout << "'" << line << "' at line " << lineNumber << std::endl;
    }
    else if(keyword == "obj_info") {
      std::cout << "'" << line << "' at line " << lineNumber << std::endl;
    }
    else if(keyword == "end_header") {
      break;
    }
  }

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
