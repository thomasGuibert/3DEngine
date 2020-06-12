#pragma once
#include <glm/glm.hpp>

#include <Geometry.h>

class GeometryFactory
{
public:
    ~GeometryFactory();
    static Geometry CreateCube(float size = 1.0f);
    static Geometry CreateSphere(float radius = 1.0f, int sub = 3);
};

