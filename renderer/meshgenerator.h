#ifndef MESHGENERATOR_H
#define MESHGENERATOR_H

class Mesh;

class MeshGenerator
{
  Mesh* plate(float width = 1.0, float heigh = 1.0);
  Mesh* cube(float width = 1.0, float height = 1.0, float depth = 1.0);
  Mesh* sphere(float radius = 1.0);
};

#endif // MESHGENERATOR_H
