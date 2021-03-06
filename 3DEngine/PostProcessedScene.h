#pragma once
#include <DefaultScene.h>
#include <ImageTexture.h>
#include <camera.h>
#include <Shader.h>

class PostProcessedScene : public DefaultScene
{
public:
    Model* screenSquare;

    PostProcessedScene(BaseCameraBehavior& manipulator);
    void render();
    ~PostProcessedScene();
private:
    unsigned int _framebuffer, _textureColorbuffer, _rbo;
    Shader _screenShader;
    glm::vec3 _positions[1] = {
        glm::vec3(0.0f,  0.0f,  0.0f)
    };
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

