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
#include <ImportedModel.h>
#include <DefaultScene.h>
#include <EmptyScene.h>
#include <PostProcessedScene.h>
#include <VoxelScene.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

GLFWwindow* create_window();
void cleanup(GLFWwindow * window);
void processInputForNextIteration(GLFWwindow *window);
void UpdateCameraPosition(GLFWwindow * window);
void frameBufferSize_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
CameraFPS camera;
Scene* scene;


int main()
{
    GLFWwindow* window = create_window();
    scene = new VoxelScene(camera);

    while (!glfwWindowShouldClose(window))
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        scene->render();

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

    UpdateCameraPosition(window);

    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetScrollCallback(window, scroll_callback);
}

void UpdateCameraPosition(GLFWwindow * window)
{
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    const float cameraSpeed = 2.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
        camera.ENABLE_HIHGLIGHT  = !camera.ENABLE_HIHGLIGHT;

    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        scene = new EmptyScene(camera);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        scene = new DefaultScene(camera);

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        scene = new PostProcessedScene(camera);

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        scene = new VoxelScene(camera);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(Direction::FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(Direction::BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(Direction::LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(Direction::RIGHT, deltaTime);
}

void cleanup(GLFWwindow * window)
{
    glfwSwapBuffers(window);
    glfwPollEvents();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.scrollUpdate(yoffset);
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos) {
    camera.mouseUpdate(xpos, ypos);
}


