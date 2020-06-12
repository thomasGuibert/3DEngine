#pragma once
#include "Scene.h"
#include "Shader.h"
#include "GL_Renderer.h"
#include "Geometry.h"
#include "GeometryFactory.h"

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
        -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  0.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  0.0f,  1.0f, 1.0f
    };
};

