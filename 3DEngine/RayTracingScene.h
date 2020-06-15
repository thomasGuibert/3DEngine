#pragma once
#include "Scene.h"
#include "Shader.h"
#include "Ray.h"
#include "GL_Renderer.h"
#include "Geometry.h"
#include "GeometryFactory.h"

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

    void render();
private:
    void setupRayTracedTexture();
    void setupQuadScreen();

    Geometry _cube;
    Geometry _sphere;
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

