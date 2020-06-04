#pragma once
#include<Shader.h>
#include<Mesh.h>
#include<string>
#include<vector>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<ImageTexture.h>

class MeshLoader
{
public:
    ~MeshLoader();
    static std::vector<Mesh> loadMesh(std::string path);
private:
    static std::vector<Mesh> meshes;

    static void processNode(aiNode *node, const aiScene *scene);
    static Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    static std::vector<ImageTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
        std::string typeName);
};
