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
#include <BaseCameraBehavior.h>
#include <DefaultCameraBehavior.h>
#include <Shader.h>
#include <DefaultScene.h>
#include <EmptyScene.h>
#include <PostProcessedScene.h>
#include <VoxelScene.h>
#include <RayTracingScene.h>
#include <TextRenderer.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

GLFWwindow* create_window();
void cleanup(GLFWwindow * window);
void processInputForNextIteration(GLFWwindow *window);
void ProcessKeyboardInputs(GLFWwindow * window);
void frameBufferSize_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
Camera camera;
BaseCameraBehavior* manipulator;
Scene* scene;
TextRenderer* textRenderer;

int main()
{
    GLFWwindow* window = create_window();
    manipulator = new VoxelCameraBehavior(camera);
    textRenderer = new TextRenderer();

    scene = new VoxelScene(static_cast<VoxelCameraBehavior&>(*manipulator));
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double elaspsedTime = currentTime - lastTime;
        nbFrames++;
        if (elaspsedTime >= 1.0) { // If last prinf() was more than 1 sec ago
            nbFrames = 0;
            lastTime += 1.0;
        }

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        processInputForNextIteration(window);
        scene->render();
        int fps = (nbFrames) / elaspsedTime;
        int spf = 1000.0 / double(nbFrames);
        std::stringstream message;
        message << fps << "FPS (" << spf << "ms/frame)";
        textRenderer->renderText(message.str(), 550.0f, 550.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

        cleanup(window);
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

    ProcessKeyboardInputs(window);

    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetScrollCallback(window, scroll_callback);
}

void ProcessKeyboardInputs(GLFWwindow * window)
{
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    const float cameraSpeed = 2.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        manipulator = new DefaultCameraBehavior(camera);
        scene = new EmptyScene(static_cast<DefaultCameraBehavior&>(*manipulator));
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        manipulator = new DefaultCameraBehavior(camera);
        scene = new DefaultScene(static_cast<DefaultCameraBehavior&>(*manipulator));
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        manipulator = new DefaultCameraBehavior(camera);
        scene = new PostProcessedScene(static_cast<DefaultCameraBehavior&>(*manipulator));
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        manipulator = new DefaultCameraBehavior(camera);
        scene = new RayTracingScene(static_cast<DefaultCameraBehavior&>(*manipulator));
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        manipulator = new VoxelCameraBehavior(camera);
        scene = new VoxelScene(static_cast<VoxelCameraBehavior&>(*manipulator));
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        manipulator->processKeyboardDirection(Direction::FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        manipulator->processKeyboardDirection(Direction::BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        manipulator->processKeyboardDirection(Direction::LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        manipulator->processKeyboardDirection(Direction::RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        manipulator->processKeyboardAction(Action::ADD);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        manipulator->processKeyboardAction(Action::REMOVE);
}

void cleanup(GLFWwindow * window)
{
    glfwSwapBuffers(window);
    glfwPollEvents();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.scrollUpdate(yoffset);
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
    camera.mouseUpdate(xpos, ypos);
}


