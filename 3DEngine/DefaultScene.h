#pragma once
#include <Scene.h>
#include <Model.h>
#include <ImageTexture.h>
#include <camera.h>
#include <Shader.h>
#include <MeshLoader.h>
#include <Skybox.h>
#include <vector>
#include <iostream>
#include <Geometry.h>
#include <GeometryFactory.h>

class DefaultScene : public Scene
{
public:
    Model* _teapot;
    Model* _teapotHighlight;
    Model* _crate;
    Model* _lightSource;
    Skybox* skybox;

    DefaultScene(BaseCameraBehavior& manipulator);
    ~DefaultScene();

    void render();

private:
    std::vector<PointLight> buildPointLights();
    DirectionalLight buildDirectionalLight();
    void setupCrate(std::vector<PointLight> &pointLights, const DirectionalLight &directionalLight);
    void setupTeapot(std::vector<PointLight> &pointLights, const DirectionalLight &directionalLight);
    void drawTeapot();
    void drawCrates();
    void drawLights();
    void updateGlobalShaderBuffer();

    unsigned int _uboGlobalVariables;
    Geometry _cube;
    Geometry _sphere;
    std::vector<Vertex> _cubeVertices = {
        //   Position                      Normal                        Texture Coords
        {
            //Front
                {glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec2(0.0f, 1.0f)},

                //Back
                    {glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
                    {glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
                    {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
                    {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

                    //Left
                        {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                        {glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                        {glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                        {glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},

                        //Right
                            {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                            {glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                            {glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                            {glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},

                            //Bottom
                                {glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                                {glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                                {glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                                {glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},

                                //Top
                                   {glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                                   {glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                                   {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                                   {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)}
                              }
    };
    std::vector<unsigned int> _cubeIndices = {
        // front
         0, 1, 2,
         2, 3, 0,
         // back
          4, 5, 6,
          6, 7, 4,
          // left
           8, 9,10,
          10,11, 8,
          // right
          12,13,14,
          14,15,12,
          // bottom
          16,17,18,
          18,19,16,
          // top
          20,21,22,
          22,23,20
    };
    glm::vec3 _cubePositions[10] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    glm::vec3 _lightPositions[4] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };
};

