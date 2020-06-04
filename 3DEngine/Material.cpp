#include "Material.h"

Material::Material(const Material& material) : _shader(material._shader), _aspect(material._aspect)
{
    diffuseTexture = material.diffuseTexture;
    specularTexture = material.specularTexture;
}

Material::Material(Shader shader, MaterialAspect& aspect) : _shader(shader), _aspect(aspect)
{
    if (_aspect.diffuseRessourcePath != "") {
        diffuseTexture = new ImageTexture(_aspect.diffuseRessourcePath.c_str(), "texture_diffuse");
        diffuseTexture->Enable(_aspect.diffuseTextureIndex);
        _shader.updateUniformInt("material.diffuse", 0);
    }
    else {
        _shader.updateUniformVec3("material.ambient", aspect.ambientColor);
        _shader.updateUniformVec3("material.diffuse", aspect.diffuseColor);

    }
    if (_aspect.specularRessourcePath != "") {
        specularTexture = new ImageTexture(_aspect.specularRessourcePath.c_str(), "texture_specular");
        specularTexture->Enable(_aspect.specularTextureIndex);
        _shader.updateUniformInt("material.specular", 1);
    }
    else {
        _shader.updateUniformVec3("material.specular", aspect.specularColor);

    }

    //_textures.push_back(diffuseTexture);
    //_textures.push_back(specularTexture);

    _shader.updateUniformFloat("material.shininess", aspect.shininess);
}

void Material::applyLights(std::vector<PointLight> lights)
{
    for (int i = 0, size = lights.size(); i < size; ++i) {
        std::string currentLight = "light[" + std::to_string(i) + "]";
        _shader.updateUniformVec3(currentLight + ".position", lights[i].position);
        _shader.updateUniformVec3(currentLight + ".ambient", lights[i].ambientColor);
        _shader.updateUniformVec3(currentLight + ".diffuse", lights[i].diffuseColor);
        _shader.updateUniformVec3(currentLight + ".specular", lights[i].specularColor);
        _shader.updateUniformFloat(currentLight + ".constant", lights[i].constant);
        _shader.updateUniformFloat(currentLight + ".linear", lights[i].linear);
        _shader.updateUniformFloat(currentLight + ".quadratic", lights[i].quadratic);
    }
}

void Material::applyLight(DirectionalLight light)
{
    _shader.updateUniformVec3("globalLight.direction", light.direction);
    _shader.updateUniformVec3("globalLight.ambient", light.ambientColor);
    _shader.updateUniformVec3("globalLight.diffuse", light.diffuseColor);
    _shader.updateUniformVec3("globalLight.specular", light.specularColor);
}

void Material::render(glm::mat4 model) {
    //diffuseTexture->Enable(_aspect.diffuseTextureIndex);
    //specularTexture->Enable(_aspect.specularTextureIndex);
    glUseProgram(_shader.shaderProgramId);

    if (diffuseTexture != NULL)
        diffuseTexture->Enable(0);
    if (specularTexture != NULL)
        specularTexture->Enable(1);

    _shader.updateUniformMat4("model", model);
    //_textures[0].Enable(0);
    //_textures[1].Enable(1);
}

Material::~Material()
{
}
