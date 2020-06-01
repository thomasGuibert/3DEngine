#include "GL_Renderer.h"

GL_Renderer::~GL_Renderer()
{
}

GL_Renderer* GL_Renderer::_instance = NULL;

GL_Renderer* GL_Renderer::Instance()
{
    if (!_instance)
        _instance = new GL_Renderer();
    return _instance;
}

void GL_Renderer::fillBufferData(unsigned int vao, std::vector<float> vertices)
{
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

void GL_Renderer::bindVertexAttrib(const unsigned int index, const AttribType attributType, const unsigned int strideLength, const unsigned int offset)
{
    glVertexAttribPointer(index, attributType, GL_FLOAT, GL_FALSE, strideLength * sizeof(float), (void*)(offset * sizeof(float)));
    glEnableVertexAttribArray(index);
}
