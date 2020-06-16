#include "RayTracingSCene.h"

unsigned int samples_per_pixel = 20;
float gamma = 0.5f;
unsigned int depth = 20;
int sphereCount = 10;

RayTracingScene::RayTracingScene(BaseCameraBehavior& manipulator) : Scene(manipulator),
_rayTracingShader("shaders/RayTracingVertexShader .vs", "shaders/RayTracingFragmentShader.fs"),
_texture(ImageTexture(SCR_WIDTH, SCR_HEIGHT))
{
    setupQuadScreen();

    manipulator.getCamera().setPosition(glm::vec3(0, 2, 3));
    glm::vec3 origin = manipulator.getCamera().getPosition();
    glm::vec3 horizontal = glm::vec3((4.0f / 3.0f) * 2.0f, 0, 0);
    glm::vec3 vertical = glm::vec3(0, 2.0, 0);
    glm::vec3 lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - glm::normalize(origin- glm::vec3(0, 0, 1));

    HittableList world;
    world.add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 1, std::make_shared<Lambertian>(glm::vec4(1, 0.5, 0.3, 1.0))));
    world.add(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100, std::make_shared<Lambertian>(glm::vec4(0.8, 0.8, 0.0, 1.0))));
    world.add(std::make_shared<Sphere>(glm::vec3(2.0, 0, -1), 1, std::make_shared<Metal>(glm::vec4(.8, .6, .2, 1.0), 1.0)));
    world.add(std::make_shared<Sphere>(glm::vec3(-1.5, 0, -1), 0.5, std::make_shared<Metal>(glm::vec4(.8, .8, .8, 1.0), 0.5)));
    world.add(std::make_shared<Sphere>(glm::vec3(-3.5, 0, -1), 1, std::make_shared<Dielectric>(1.5)));
    world.add(std::make_shared<Sphere>(glm::vec3(3.5, 0, -1), -0.45, std::make_shared<Dielectric>(1.5)));

    for (int a = -sphereCount/2; a < sphereCount/2; a++) {
        for (int b = -sphereCount/2; b < sphereCount/2; b++) {
            float choose_mat = getRandomFloat();
            glm::vec3 center(a + 0.9*getRandomFloat(), -0.2, b + 0.9*getRandomFloat());
    
            if ((center - glm::vec3(4, 0.0, 0)).length() > 0.9) {
                std::shared_ptr<RT_Material> sphere_material;
    
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = glm::vec4(getRandomFloat(), getRandomFloat(), getRandomFloat(), 1.0f);
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = glm::vec4(getRandomFloat(), getRandomFloat(), getRandomFloat(), 1.0f);
                    auto fuzz = getRandomFloat(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }



    for (unsigned int i = 0; i < SCR_HEIGHT; ++i) {
        std::cerr << "\rScanlines remaining: " << SCR_HEIGHT - i - 1 << ' ' << std::flush;
        for (unsigned int j = 0; j < SCR_WIDTH; ++j) {

            glm::vec4 pixelColor(0.0f, 0.0f, 0.0f, 1.0f);
            int s = 1;
            do {
                float u = (j + RayTracingScene::getRandomFloat()) / (SCR_WIDTH - 1);
                float v = (i + RayTracingScene::getRandomFloat()) / (SCR_HEIGHT - 1);
                Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
                pixelColor += RayTracingScene::ray_color(r, world, depth);
                ++s;
            } while (s < samples_per_pixel);

            glm::vec2 position(i, j);

            auto scale = 1.0 / samples_per_pixel;
            float r = glm::pow(RayTracingScene::clamp(pixelColor.r * scale), gamma);
            float g = glm::pow(RayTracingScene::clamp(pixelColor.g * scale), gamma);
            float b = glm::pow(RayTracingScene::clamp(pixelColor.b * scale), gamma);
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

float RayTracingScene::getRandomFloat(float min, float max) {
    static std::uniform_real_distribution<float> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

glm::vec3 RayTracingScene::getRandomUnitVector() {
    float a = getRandomFloat(0, 2 * pi);
    float z = getRandomFloat();
    float r = glm::sqrt(1 - z * z);
    return glm::vec3(r*cos(a), r*sin(a), z);
}

glm::vec3 RayTracingScene::getRandomInUnitSphere() {
    while (true) {
        glm::vec3 p = glm::vec3(getRandomFloat(), getRandomFloat(), getRandomFloat());
        if (glm::length2(p) >= 1) continue;
        return p;
    }
}

glm::vec3 RayTracingScene::getRandomInHemisphere(const glm::vec3& normal) {
    glm::vec3 in_unit_sphere = getRandomInUnitSphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}


glm::vec4 RayTracingScene::ray_color(const Ray& r, const IHittable& world, int depth) {
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0.0f)
        return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    hit_record rec;
    if (world.hit(r, 0.001f, infinity, rec)) {
        Ray scattered;
        glm::vec4 attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }

        return glm::vec4(0, 0, 0, 0);


        //glm::vec3 target = rec.position + rec.normal + getRandomUnitVector();
        //glm::vec3 target = rec.position + rec.normal + getRandomInUnitSphere();
        glm::vec3 target = rec.position + RayTracingScene::getRandomInHemisphere(rec.normal);

        glm::vec3 color = 0.5f * RayTracingScene::ray_color(Ray(rec.position, target - rec.position), world, depth - 1);

        return glm::vec4(color.r, color.g, color.b, 1.0f);
    }
    glm::vec3 unit_direction = glm::normalize(r.direction());
    float t = 0.5f*(unit_direction.y + 1.0f);
    glm::vec3 color = (1.0f - t)*glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
    return glm::vec4(color.r, color.g, color.b, 1.0f);
}

float RayTracingScene::clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}