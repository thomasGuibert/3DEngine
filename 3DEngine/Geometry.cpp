#include "Geometry.h"



Geometry::Geometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<ImageTexture> textures) :
    _vertices(vertices),
    _indices(indices),
    _textures(textures)
{

}

Geometry::Geometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices) :
    _vertices(vertices),
    _indices(indices)
{

}

Geometry::~Geometry()
{
}

std::vector<Vertex> Geometry::getVertices()
{
    return _vertices;
}

std::vector<unsigned int> Geometry::getIndices()
{
    return _indices;
}

std::vector<ImageTexture> Geometry::getTextures()
{
    return _textures;
}
