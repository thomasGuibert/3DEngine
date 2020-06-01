#pragma once
#include <DefaultScene.h>
#include <ImageTexture.h>
#include <camera.h>
#include <Shader.h>
#include <Model.h>
#include <ImportedModel.h>

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
    float _quadVertices[30] = {
        // positions          // texCoords
        -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  0.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  0.0f,  1.0f, 1.0f
    };
};

