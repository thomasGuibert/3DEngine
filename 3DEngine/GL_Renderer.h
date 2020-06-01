#pragma once
#include <glad/glad.h>
#include <vector>

enum AttribType {
    SCALAR = 1,
    VEC2 = 2,
    VEC3 = 3,
    VEC4 = 4
};
class GL_Renderer
{
public:
    static GL_Renderer* Instance();

    void fillBufferData(unsigned int vao, const std::vector<float> vertices);
    void bindVertexAttrib(const unsigned int index, const AttribType atrtibutType, const unsigned int stride, const unsigned int offset);

private:
    GL_Renderer& operator= (const GL_Renderer&) {};
    GL_Renderer(const GL_Renderer&) {};
    GL_Renderer() {};
    ~GL_Renderer();
    static GL_Renderer* _instance;
};


