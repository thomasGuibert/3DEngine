#include "Model.h"

Model::Model(std::string path, Material& material)
{
    meshes = MeshLoader::loadMesh(path);
    for (unsigned int i = 0; i < meshes.size(); i++)
        materials.push_back(material);
}

Model::Model(Mesh& model, Material& material)
{
    meshes.push_back(model);
    materials.push_back(material);
}

void Model::setModelMatrix(glm::mat4 model)
{
    _model = model;
}

void Model::draw()
{
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(materials[i], _model);
    }
}

Model::~Model()
{
}
