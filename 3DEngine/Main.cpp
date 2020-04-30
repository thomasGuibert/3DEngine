#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ImageTexture.h>
#include <camera.h>
#include <Shader.h>
#include <Model.h>

GLFWwindow* create_window();
void cleanup(GLFWwindow * window);
void processInputForNextIteration(GLFWwindow *window);
void frameBufferSize_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
Camera camera;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
};

glm::vec3 cubePositions[] = {
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

glm::vec3 lightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};

int main()
{
    GLFWwindow* window = create_window();

    glm::vec3 pointLightColor = glm::vec3(0.0f, 1.0f, 1.0f);
    glm::vec3 globalLight = glm::vec3(1.0f, 1.0f, 1.0f);

    ImageTexture crateTexture("../Assets/containerWithIron.png");
    crateTexture.Enable(0);

    ImageTexture steelCrateTexture("../Assets/containerWithIron_specular.png");
    steelCrateTexture.Enable(1);

    Shader crateShader("./shaders/CubeVertexShader.vs", "./shaders/CubeFragmentShader.fs");
    crateShader.updateUniformInt("material.diffuse", 0);
    crateShader.updateUniformInt("material.specular", 1);
    crateShader.updateUniformVec3("material.specular", 0.5f, 0.5f, 0.5f);
    crateShader.updateUniformFloat("material.shininess", 8.0f);

    for (int i = 0, lightPointsCount = sizeof(lightPositions) / sizeof(glm::vec3); i < lightPointsCount; ++i) {
        std::string currentLight = "light[" + std::to_string(i) + "]";
        crateShader.updateUniformVec3(currentLight + "position", lightPositions[i]);
        crateShader.updateUniformVec3(currentLight + ".ambient", pointLightColor);
        crateShader.updateUniformVec3(currentLight + ".diffuse", pointLightColor);
        crateShader.updateUniformVec3(currentLight + ".specular", 0.2f, 1.0f, 1.0f);
        crateShader.updateUniformFloat(currentLight + ".constant", 1.0f);
        crateShader.updateUniformFloat(currentLight + ".linear", 0.07f);
        crateShader.updateUniformFloat(currentLight + ".quadratic", 0.017f);
    }

    crateShader.updateUniformVec3("globalLight.direction", -0.2f, -1.0f, -0.3f);
    crateShader.updateUniformVec3("globalLight.ambient", globalLight);
    crateShader.updateUniformVec3("globalLight.diffuse", globalLight);
    crateShader.updateUniformVec3("globalLight.specular", globalLight);

    crateShader.updateUniformVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

    Shader lightSourceShader("./shaders/LightSourceVertexShader.vs", "./shaders/LightSourceFragmentShader.fs");
    lightSourceShader.updateUniformVec3("lightColor", pointLightColor);

    Model crate(vertices, sizeof(vertices) / sizeof(float), crateShader);

    Model lightSource(vertices, sizeof(vertices) / sizeof(float), lightSourceShader);
    lightSource.setScale(glm::vec3(0.2f));

    glm::mat4 view;
    glm::mat4 projection;

    while (!glfwWindowShouldClose(window))
    {
        projection = camera.perspective();
        crateShader.updateUniformMat4("projection", projection);
        lightSourceShader.updateUniformMat4("projection", projection);

        view = camera.lookAt();
        crateShader.updateUniformMat4("view", view);
        lightSourceShader.updateUniformMat4("view", view);


        lightSource.drawOnPositions(lightPositions, sizeof(lightPositions) / sizeof(glm::vec3));
        crate.drawOnPositions(cubePositions, sizeof(cubePositions) / sizeof(glm::vec3));

        glm::vec3 cameraPosition = camera.getPosition();
        crateShader.updateUniformVec3("viewPos", cameraPosition);

        glEnable(GL_DEPTH_TEST);
        Sleep(100);

        cleanup(window);
        processInputForNextIteration(window);
    }

    glfwTerminate();
    return 0;
}

GLFWwindow* create_window()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::string("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw std::string("Failed to initialize GLAD");
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, frameBufferSize_callback);
    return window;
}

void frameBufferSize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInputForNextIteration(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    const float cameraSpeed = 2.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(Direction::FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(Direction::BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(Direction::LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(Direction::RIGHT, deltaTime);

    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (state == GLFW_PRESS)
        glfwSetCursorPosCallback(window, mouse_callback);
    else {
        camera.isMousePressed = false;
        glfwSetCursorPosCallback(window, NULL);
    }

    glfwSetScrollCallback(window, scroll_callback);
}

void cleanup(GLFWwindow * window)
{
    glfwSwapBuffers(window);
    glfwPollEvents();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.scrollUpdate(yoffset);
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos) {
    camera.mouseUpdate(xpos, ypos);
}


