#include "DefaultScene.h"

DefaultScene::DefaultScene(Camera &camera): Scene(camera),

_crateShader("./shaders/CubeVertexShader.vs", "./shaders/CubeFragmentShader.fs"),
_teapotShader("./shaders/teapotVertexShader.vs", "./shaders/teapotFragmentShader.fs"),
_lightSourceShader("./shaders/LightSourceVertexShader.vs", "./shaders/LightSourceFragmentShader.fs")
{
    glm::vec3 pointLightColor = glm::vec3(0.0f, 1.0f, 1.0f);
    glm::vec3 globalLight = glm::vec3(1.0f, 1.0f, 1.0f);

    _crateTexture = new ImageTexture("../Assets/containerWithIron.png", "texture_diffuse");
    _crateTexture->Enable(0);
    _steelCrateTexture = new ImageTexture("../Assets/containerWithIron_specular.png", "texture_specular"),
    _steelCrateTexture->Enable(1);

    _crateShader.updateUniformInt("material.diffuse", 0);
    _crateShader.updateUniformInt("material.specular", 1);
    _crateShader.updateUniformVec3("material.specular", 0.5f, 0.5f, 0.5f);
    _crateShader.updateUniformFloat("material.shininess", 8.0f);

    for (int i = 0, lightPointsCount = sizeof(_lightPositions) / sizeof(glm::vec3); i < lightPointsCount; ++i) {
        std::string currentLight = "light[" + std::to_string(i) + "]";
        _crateShader.updateUniformVec3(currentLight + ".position", _lightPositions[i]);
        _crateShader.updateUniformVec3(currentLight + ".ambient", pointLightColor);
        _crateShader.updateUniformVec3(currentLight + ".diffuse", pointLightColor);
        _crateShader.updateUniformVec3(currentLight + ".specular", 0.2f, 1.0f, 1.0f);
        _crateShader.updateUniformFloat(currentLight + ".constant", 1.0f);
        _crateShader.updateUniformFloat(currentLight + ".linear", 0.07f);
        _crateShader.updateUniformFloat(currentLight + ".quadratic", 0.017f);
    }

    _crateShader.updateUniformVec3("globalLight.direction", -0.2f, -1.0f, -0.3f);
    _crateShader.updateUniformVec3("globalLight.ambient", globalLight);
    _crateShader.updateUniformVec3("globalLight.diffuse", globalLight);
    _crateShader.updateUniformVec3("globalLight.specular", globalLight);

    _crateShader.updateUniformVec3("viewPos", _camera.getPosition().x, _camera.getPosition().y, _camera.getPosition().z);

    _teapotShader.updateUniformVec3("material.ambient", 0.1f, 0.0f, 0.3f);
    _teapotShader.updateUniformVec3("material.diffuse", 0.3f, 0.0f, 0.3f);
    _teapotShader.updateUniformVec3("material.specular", 1.0f, 0.2f, 1.0f);
    _teapotShader.updateUniformFloat("material.shininess", 8.0f);

    for (int i = 0, lightPointsCount = sizeof(_lightPositions) / sizeof(glm::vec3); i < lightPointsCount; ++i) {
        std::string currentLight = "light[" + std::to_string(i) + "]";
        _teapotShader.updateUniformVec3(currentLight + ".position", _lightPositions[i]);
        _teapotShader.updateUniformVec3(currentLight + ".ambient", pointLightColor);
        _teapotShader.updateUniformVec3(currentLight + ".diffuse", pointLightColor);
        _teapotShader.updateUniformVec3(currentLight + ".specular", 0.2f, 1.0f, 1.0f);
        _teapotShader.updateUniformFloat(currentLight + ".constant", 1.0f);
        _teapotShader.updateUniformFloat(currentLight + ".linear", 0.07f);
        _teapotShader.updateUniformFloat(currentLight + ".quadratic", 0.017f);
    }

    _teapotShader.updateUniformVec3("globalLight.direction", -0.2f, -1.0f, -0.3f);
    _teapotShader.updateUniformVec3("globalLight.ambient", globalLight);
    _teapotShader.updateUniformVec3("globalLight.diffuse", globalLight);
    _teapotShader.updateUniformVec3("globalLight.specular", globalLight);

    _teapotShader.updateUniformVec3("viewPos", _camera.getPosition().x, _camera.getPosition().y, _camera.getPosition().z);

    _lightSourceShader.updateUniformVec3("lightColor", pointLightColor);

    teapot = new ImportedModel("../Assets/teapot.obj", _teapotShader);
    crate = new Model(_vertices, sizeof(_vertices) / sizeof(float), _crateShader);
    lightSource = new Model(_vertices, sizeof(_vertices) / sizeof(float), _lightSourceShader);
    lightSource->setScale(glm::vec3(0.2f));
}

void DefaultScene::render()
{
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 cameraPosition;
    
    projection = _camera.perspective();
    _crateShader.updateUniformMat4("projection", projection);
    _lightSourceShader.updateUniformMat4("projection", projection);
    _teapotShader.updateUniformMat4("projection", projection);
    
    view = _camera.lookAt();
    _crateShader.updateUniformMat4("view", view);
    _lightSourceShader.updateUniformMat4("view", view);
    _teapotShader.updateUniformMat4("view", view);
    
    teapot->draw();
    lightSource->drawOnPositions(_lightPositions, sizeof(_lightPositions) / sizeof(glm::vec3));
    crate->drawOnPositions(_cubePositions, sizeof(_cubePositions) / sizeof(glm::vec3));
    cameraPosition = _camera.getPosition();
    
    _crateShader.updateUniformVec3("viewPos", cameraPosition);
}


DefaultScene::~DefaultScene()
{
}
