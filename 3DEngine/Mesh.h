#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader.h>
#include <ImageTexture.h>
#include <Material.h>

#include <string>
#include <vector>

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

class Mesh
{
public:
    std::vector<Vertex>       _vertices;
    std::vector<unsigned int> _indices;
    std::vector<ImageTexture> _textures;
    unsigned int VAO;
    Mesh();
    //Mesh(const Mesh& mesh);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<ImageTexture> textures);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    void Draw(Material& material, glm::mat4 model);

    ~Mesh();

private:
    unsigned int VBO, EBO;

    void setupMesh();
};

