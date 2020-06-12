#include "RayTracingSCene.h"
#include <iostream>
RayTracingScene::RayTracingScene(BaseCameraBehavior& manipulator): Scene(manipulator), 
_cube(GeometryFactory::CreateCube()), 
_sphere(GeometryFactory::CreateSphere(1.0f, 3)),
_rayTracingShader("shaders/RayTracingVertexShader .vs", "shaders/RayTracingFragmentShader.fs"),
_texture(ImageTexture (SCR_WIDTH, SCR_HEIGHT))
{
    setupQuadScreen();

    for (unsigned int i = 0; i < SCR_HEIGHT; ++i) {
        for (unsigned int j = 0; j < SCR_WIDTH; ++j) {
            glm::vec2 position(i, j);
            glm::vec4 color(0.0f, 1.0f, 1.0f, 1.0f);
            _texture.setPixel(position, color);
        }
    }

    _texture.buildImage();
}

void RayTracingScene::setupQuadScreen()
{
    unsigned int quadVBO;
    glGenVertexArrays(1, &_quadVAO);
    GL_Renderer::Instance()->fillBufferData(_quadVAO, _quadVertices);
    GL_Renderer::Instance()->bindVertexAttrib(0, AttribType::VEC3, 5, 0);
    GL_Renderer::Instance()->bindVertexAttrib(1, AttribType::VEC2, 5, 3);

    _texture.Enable(0);
    _rayTracingShader.updateUniformInt("rayTracingTexture", 0);
}

RayTracingScene::~RayTracingScene()
{
}

void RayTracingScene::render()
{
    glBindVertexArray(_quadVAO);
    glUseProgram(_rayTracingShader.shaderProgramId);

    _texture.Enable(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
