#pragma once
#include "Scene.h"
#include "Shader.h"
#include "Ray.h"
#include "GL_Renderer.h"
#include "Geometry.h"
#include "GeometryFactory.h"
#include "IHittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"
#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>

#include <iostream>
#include <memory>

#include <random>

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}
class RayTracingScene :
    public Scene
{
public:
    RayTracingScene(BaseCameraBehavior& manipulator);
    ~RayTracingScene();
    static float getRandomFloat(float min = 0.0f, float max = 1.0f);
    static glm::vec3 getRandomUnitVector();
    static glm::vec3 getRandomInHemisphere(const glm::vec3& normal);
    static glm::vec3 getRandomInUnitSphere();
    static float clamp(float x, float min = 0.0f, float max = 0.999f);
    static glm::vec4 ray_color(const Ray& r, const IHittable& world, int depth);

    void render();
private:
    void setupQuadScreen();

    Shader _rayTracingShader;
    unsigned int _textureRayTracing;
    unsigned int _quadVAO;
    ImageTexture _texture;
    std::vector<float> _quadVertices = {
        // positions          // texCoords
        -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 1.0f
    };
};

