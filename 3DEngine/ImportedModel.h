#pragma once
#include<Shader.h>
#include<Mesh.h>
#include<string>
#include<vector>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include <ImageTexture.h>


class ImportedModel
{
public:
    ImportedModel(std::string path, Shader shader);
    void draw();
    ~ImportedModel();

private:
    // model data
    Shader _shader;
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<ImageTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
        std::string typeName);
};
