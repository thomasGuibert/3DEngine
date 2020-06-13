#include "RayTracingSCene.h"
#include <iostream>

glm::vec4 ray_color(const Ray& r) {
    glm::vec3 unit_direction = glm::normalize(r.direction());
    float t = 0.5f * (unit_direction.y + 1.0f);
    return glm::vec4((1.0f - t)*glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0),1.0);
}

RayTracingScene::RayTracingScene(BaseCameraBehavior& manipulator): Scene(manipulator), 
_cube(GeometryFactory::CreateCube()), 
_sphere(GeometryFactory::CreateSphere(1.0f, 3)),
_rayTracingShader("shaders/RayTracingVertexShader .vs", "shaders/RayTracingFragmentShader.fs"),
_texture(ImageTexture (SCR_WIDTH, SCR_HEIGHT))
{
    setupQuadScreen();

    glm::vec3 origin = glm::vec3(0, 0, 0);
    glm::vec3 horizontal = glm::vec3(2.0, 0, 0);
    glm::vec3 vertical = glm::vec3(0, 2.0, 0);
    glm::vec3 lower_left_corner = origin - horizontal/2.0f - vertical/2.0f - glm::vec3(0, 0, 1);


    for (unsigned int i = 0; i < SCR_HEIGHT; ++i) {
        std::cerr << "\rScanlines remaining: " << SCR_HEIGHT-i-1 << ' ' << std::flush;
        for (unsigned int j = 0; j < SCR_WIDTH; ++j) {
            glm::vec2 position(i, j);
            float u = double(j) / (SCR_WIDTH - 1);
            float v = double(i) / (SCR_HEIGHT - 1);
            Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            glm::vec4 pixel_color = ray_color(r);

            
            //glm::vec4 color(0.0f, 1.0f, 1.0f, 1.0f);
            _texture.setPixel(position, pixel_color);
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
