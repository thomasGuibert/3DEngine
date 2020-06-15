#include "RayTracingSCene.h"
#include "IHittable.h"
#include "HittableList.h"
#include "Sphere.h"

#include <iostream>
#include <memory>

float hit_sphere(const glm::vec3& center, double radius, const Ray& r) {
    glm::vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a*c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-b - sqrt(discriminant)) / (2.0*a);
    }
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

    for (unsigned int i = 0; i < SCR_HEIGHT; ++i) {
        std::cerr << "\rScanlines remaining: " << SCR_HEIGHT - i - 1 << ' ' << std::flush;
        for (unsigned int j = 0; j < SCR_WIDTH; ++j) {
            glm::vec2 position(i, j);
            float u = float(j) / (SCR_WIDTH - 1);
            float v = float(i) / (SCR_HEIGHT - 1);
            Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            glm::vec4 pixel_color = ray_color(r, world);


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
