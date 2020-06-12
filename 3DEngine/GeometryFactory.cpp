#include "GeometryFactory.h"



GeometryFactory::~GeometryFactory()
{
}

Geometry GeometryFactory::CreateCube()
{
    return Geometry(cubeVertices, cubeIndices);
}

const std::vector<Vertex> GeometryFactory::cubeVertices = {
    //   Position                      Normal                        Texture Coords
    {
        //Front
        {glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec2(0.0f, 1.0f)},

        //Back
        {glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

        //Left
        {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},

        //Right
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},

        //Bottom
        {glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},

        //Top
        {glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)}
    }
};

const std::vector<unsigned int> GeometryFactory::cubeIndices = {
    // front
     0, 1, 2,
     2, 3, 0,
     // back
      4, 5, 6,
      6, 7, 4,
      // left
       8, 9,10,
      10,11, 8,
      // right
      12,13,14,
      14,15,12,
      // bottom
      16,17,18,
      18,19,16,
      // top
      20,21,22,
      22,23,20
};