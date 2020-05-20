#include "GL_Block.h"

void GL_Block::addFace(const float* faceVertices, const float* faceNormals, glm::vec3 offset)
{
    unsigned int verticesCount = 6;
    unsigned int faceVerticeIndex = 0;
    unsigned int textureVerticeIndex = 0;
    unsigned int normalVerticeIndex = 0;

    float outVertices[18];
    translate(faceVertices, outVertices, offset);
    for (unsigned int i = 0; i < verticesCount; ++i) {
        _vertices.push_back(outVertices[faceVerticeIndex++]);
        _vertices.push_back(outVertices[faceVerticeIndex++]);
        _vertices.push_back(outVertices[faceVerticeIndex++]);

        _vertices.push_back(TEXTURE.COORDS[textureVerticeIndex++]);
        _vertices.push_back(TEXTURE.COORDS[textureVerticeIndex++]);

        _vertices.push_back(faceNormals[normalVerticeIndex++]);
        _vertices.push_back(faceNormals[normalVerticeIndex++]);
        _vertices.push_back(faceNormals[normalVerticeIndex++]);
    }
}

void GL_Block::translate(const float vertices[], float outVertices[], glm::vec3 offset) {
    for (int i = 0; i < 18; i += 3) {
        outVertices[i] = vertices[i] + offset.x;
        outVertices[i + 1] = vertices[i + 1] + offset.y;
        outVertices[i + 2] = vertices[i + 2] + offset.z;
    }
}

void GL_Block::build()
{
    unsigned int VBO;
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void GL_Block::render(Shader& shader, glm::vec3 position)
{
    glBindVertexArray(_VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    shader.updateUniformMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
}


GL_Block::~GL_Block()
{
}
