#pragma once
#include "Shader.h"
#include "ImageTexture.h"
#include <vector>

struct MaterialAspect {
    glm::vec3 ambientColor;
    unsigned int ambientTextureIndex;

    glm::vec3 diffuseColor;
    std::string diffuseRessourcePath;
    unsigned int diffuseTextureIndex;

    glm::vec3 specularColor;
    std::string specularRessourcePath;
    unsigned int specularTextureIndex;

    float shininess;
};

struct Light {
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
};

struct PointLight : public Light {
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight : public Light {
    glm::vec3 direction;
};

class Material
{
public:
    Shader _shader;

    Material(const Material& material);
    Material(Shader shader, MaterialAspect& description);
    ~Material();

    void applyLights(std::vector<PointLight> lights);
    void applyLight(DirectionalLight light);
    void render(glm::mat4 model);
    std::vector<ImageTexture> _textures;
    ImageTexture* diffuseTexture;
    ImageTexture* specularTexture;
private:

    MaterialAspect& _aspect;
};

