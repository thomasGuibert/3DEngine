#include "RayTracingSCene.h"
#include "IHittable.h"
#include "HittableList.h"
#include "Sphere.h"

#include <iostream>
#include <memory>

#include <random>

float random_float() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    //return 0.0f;
    return distribution(generator);
}

glm::vec4 ray_color(const Ray& r, const IHittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        glm::vec3 color = 0.5f * (rec.normal + glm::vec3(1.0f, 1.0f, 1.0f));
        return glm::vec4(color.r, color.g, color.b, 1.0f);
    }
    glm::vec3 unit_direction = glm::normalize(r.direction());
    float t = 0.5f*(unit_direction.y + 1.0);
    glm::vec3 color = (1.0f - t)*glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0);
    return glm::vec4(color.r, color.g, color.b, 1.0f);
}

float clamp(float x, float min = 0.0f, float max = 0.999f) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

RayTracingScene::RayTracingScene(BaseCameraBehavior& manipulator) : Scene(manipulator),
_cube(GeometryFactory::CreateCube()),
_sphere(GeometryFactory::CreateSphere(1.0f, 3)),
_rayTracingShader("shaders/RayTracingVertexShader .vs", "shaders/RayTracingFragmentShader.fs"),
_texture(ImageTexture(SCR_WIDTH, SCR_HEIGHT))
{
    setupQuadScreen();

    glm::vec3 origin = glm::vec3(0, 0, 0);
    glm::vec3 horizontal = glm::vec3((4.0f / 3.0f) * 2.0f, 0, 0);
    glm::vec3 vertical = glm::vec3(0, 2.0, 0);
    glm::vec3 lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - glm::vec3(0, 0, 1);

    HittableList world;
    world.add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100));

    unsigned int samples_per_pixel = 5;

    for (unsigned int i = 0; i < SCR_HEIGHT; ++i) {
        std::cerr << "\rScanlines remaining: " << SCR_HEIGHT - i - 1 << ' ' << std::flush;
        for (unsigned int j = 0; j < SCR_WIDTH; ++j) {

            glm::vec4 pixelColor(0.0f, 0.0f, 0.0f, 1.0f);
            int s = 1;
            do {
                float u = (j + random_float()) / (SCR_WIDTH - 1);
                float v = (i + random_float()) / (SCR_HEIGHT - 1);
                Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
                pixelColor += ray_color(r, world);
                ++s;
            } while (s < samples_per_pixel);

            glm::vec2 position(i, j);

            auto scale = 1.0 / samples_per_pixel;
            float r = clamp(pixelColor.r * scale);
            float g = clamp(pixelColor.g * scale);
            float b = clamp(pixelColor.b * scale);
            glm::vec4 clampedPixelColor(r, g, b, 1.0f);
            _texture.setPixel(position, clampedPixelColor);
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
