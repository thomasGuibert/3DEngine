#pragma once
#include <glm/glm.hpp>

#include <Geometry.h>

class GeometryFactory
{
public:
    ~GeometryFactory();
    static Geometry CreateCube();

    static const std::vector<Vertex> cubeVertices;
    static const std::vector<unsigned int> cubeIndices;
};

