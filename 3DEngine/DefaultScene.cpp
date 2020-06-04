#include "DefaultScene.h"
#include <iostream>

DefaultScene::DefaultScene(BaseCameraBehavior& manipulator) : Scene(manipulator)
{
    glGenBuffers(1, &_uboGlobalVariables);
    glBindBuffer(GL_UNIFORM_BUFFER, _uboGlobalVariables);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, _uboGlobalVariables, 0, 2 * sizeof(glm::mat4) + sizeof(glm::vec4));

    std::vector<PointLight> pointLights = buildPointLights();
    DirectionalLight directionalLight = buildDirectionalLight();;

    setupCrate(pointLights, directionalLight);
    setupTeapot(pointLights, directionalLight);

    skybox = new Skybox("../Assets/skybox/", _manipulator.getCamera());
    manipulator.getCamera().setPosition(glm::vec3(0.5, 1, 1.9));

    //_crateShader.updateUniformInt("skybox", 0);
}

DefaultScene::~DefaultScene()
{
}

void DefaultScene::render()
{
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    updateGlobalShaderBuffer();

    glStencilMask(0x00); // make sure we don't update the stencil buffer while drawing the floor
    drawLights();

    drawCrates();

    skybox->render();

    drawTeapot();

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    glm::vec3 cameraPositionOld;
    cameraPositionOld = _manipulator.getCamera().getPosition();
}

void DefaultScene::drawTeapot()
{
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 scale = glm::vec3(0.1f);
    model = glm::scale(model, scale);
    glm::vec3 translate = glm::vec3(0.0f, 5.0f, 0.0f);
    model = glm::translate(model, translate);
    _teapot->setModelMatrix(model);
    _teapot->draw();

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::vec3 scale2 = glm::vec3(0.106f, 0.112f, 0.105f);
    model2 = glm::scale(model2, scale2);
    glm::vec3 translate2 = glm::vec3(0.0f, 4.3f, 0.0f);
    model2 = glm::translate(model2, translate2);
    _teapotHighlight->setModelMatrix(model2);
    _teapotHighlight->draw();

    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
}

void DefaultScene::drawCrates()
{
    for (unsigned int i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, _cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        _crate->setModelMatrix(model);
        _crate->draw();
    }
}

void DefaultScene::drawLights()
{
    for (unsigned int i = 0; i < 4; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, _lightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f));
        _lightSource->setModelMatrix(model);
        _lightSource->draw();
    }
}

void DefaultScene::updateGlobalShaderBuffer()
{
    glm::mat4 projection = _manipulator.getCamera().perspective();
    glBindBuffer(GL_UNIFORM_BUFFER, _uboGlobalVariables);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glm::mat4 view = _manipulator.getCamera().lookAt();
    glBindBuffer(GL_UNIFORM_BUFFER, _uboGlobalVariables);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glm::vec3 viewPos = _manipulator.getCamera().getPosition();
    glBindBuffer(GL_UNIFORM_BUFFER, _uboGlobalVariables);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec4), glm::value_ptr(viewPos));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::vector<PointLight> DefaultScene::buildPointLights() {
    glm::vec3 pointLightColor = glm::vec3(0.0f, 1.0f, 1.0f);
    std::vector<PointLight> pointLights;
    for (int i = 0, lightPointsCount = sizeof(_lightPositions) / sizeof(glm::vec3); i < lightPointsCount; ++i) {
        PointLight pointLight;
        pointLight.position = _lightPositions[i];
        pointLight.ambientColor = glm::vec3(0.0f, 0.2f, 0.2f);
        pointLight.diffuseColor = glm::vec3(0.0f, 1.0f, 1.0f);
        pointLight.specularColor = glm::vec3(0.2f, 1.0f, 1.0f);
        pointLight.constant = 1.0f;
        pointLight.linear = 0.07f;
        pointLight.quadratic = 0.017f;

        pointLights.push_back(pointLight);
    }
    Shader lightSourceShader("./shaders/LightSourceVertexShader.vs", "./shaders/LightSourceFragmentShader.fs");
    lightSourceShader.updateUniformVec3("lightColor", pointLightColor);
    glUniformBlockBinding(lightSourceShader.shaderProgramId, glGetUniformBlockIndex(lightSourceShader.shaderProgramId, "Global"), 0);
    MaterialAspect lightAspect;
    lightAspect.ambientColor = pointLightColor;
    lightAspect.diffuseColor = pointLightColor;
    lightAspect.specularColor = pointLightColor;
    lightAspect.shininess = 0.0f;
    Material lightMaterial(lightSourceShader, lightAspect);
    Mesh light(_cubeVertices, _cubeIndices);
    _lightSource = new Model(light, lightMaterial);
    return pointLights;
};

DirectionalLight DefaultScene::buildDirectionalLight()
{
    glm::vec3 globalLight = glm::vec3(1.0f, 1.0f, 1.0f);
    DirectionalLight directionalLight;
    directionalLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    directionalLight.ambientColor = globalLight;
    directionalLight.diffuseColor = globalLight;
    directionalLight.specularColor = globalLight;

    return directionalLight;
}

void DefaultScene::setupCrate(std::vector<PointLight> &pointLights, const DirectionalLight &directionalLight)
{
    MaterialAspect crateAspect;
    crateAspect.diffuseRessourcePath = "../Assets/containerWithIron.png";
    crateAspect.diffuseTextureIndex = 0;
    crateAspect.specularRessourcePath = "../Assets/containerWithIron_specular.png";
    crateAspect.specularTextureIndex = 1;
    crateAspect.shininess = 256.0f;
    Shader crateShader("./shaders/CubeVertexShader.vs", "./shaders/CubeFragmentShader.fs");
    glUniformBlockBinding(crateShader.shaderProgramId, glGetUniformBlockIndex(crateShader.shaderProgramId, "Global"), 0);
    Material crateMaterial(crateShader, crateAspect);
    crateMaterial.applyLights(pointLights);
    crateMaterial.applyLight(directionalLight);
    Mesh crate(_cubeVertices, _cubeIndices);
    _crate = new Model(crate, crateMaterial);
}

void DefaultScene::setupTeapot(std::vector<PointLight> &pointLights, const DirectionalLight &directionalLight)
{
    //TEAPOT
    MaterialAspect teapotAspect;
    teapotAspect.ambientColor = glm::vec3(0.1f, 0.0f, 0.3f);
    teapotAspect.diffuseColor = glm::vec3(0.3f, 0.0f, 0.3f);
    teapotAspect.specularColor = glm::vec3(1.0f, 0.2f, 1.0f);
    teapotAspect.shininess = 8.0f;
    Shader teapotShader("./shaders/teapotVertexShader.vs", "./shaders/teapotFragmentShader.fs");
    glUniformBlockBinding(teapotShader.shaderProgramId, glGetUniformBlockIndex(teapotShader.shaderProgramId, "Global"), 0);
    Material teapotMaterial(teapotShader, teapotAspect);
    teapotMaterial.applyLights(pointLights);
    teapotMaterial.applyLight(directionalLight);
    _teapot = new Model("../Assets/teapot.obj", teapotMaterial);


    //TEAPOT HIGHLIGHT 
    MaterialAspect teapotHighlightAspect;
    teapotHighlightAspect.diffuseColor = glm::vec3(0.0f, 0.0f, 1.0f);
    Shader teapotHighlightShader("./shaders/HighlightVertexShader.vs", "./shaders/HighlightFragmentShader.fs");
    glUniformBlockBinding(teapotHighlightShader.shaderProgramId, glGetUniformBlockIndex(teapotHighlightShader.shaderProgramId, "Global"), 0);
    Material teapotHighlightMaterial(teapotHighlightShader, teapotHighlightAspect);
    _teapotHighlight = new Model("../Assets/teapot.obj", teapotHighlightMaterial);
}
