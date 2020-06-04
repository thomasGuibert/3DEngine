#pragma once
#include<Shader.h>
#include<string>
#include<MeshLoader.h>
#include "Mesh.h"

class Model
{
public:
    Model(std::string path, Material& material);
    Model(Mesh& mesh, Material& material);
    void setModelMatrix(glm::mat4 model);
    void draw();
    ~Model();
    std::vector<Mesh> meshes;
    std::vector<Material> materials;

private:
    glm::mat4 _model;

    MeshLoader* _ModelLoader;
};

