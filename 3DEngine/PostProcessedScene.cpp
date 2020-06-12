#include "PostProcessedScene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

unsigned int quadVAO;

PostProcessedScene::PostProcessedScene(BaseCameraBehavior& manipulator) : DefaultScene(manipulator),
_screenShader("shaders/ScreenVertexShader.vs", "shaders/ScreenFragmentShader.fs")
{
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

    // create a color attachment texture
    glGenTextures(1, &_textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, _textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureColorbuffer, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo); // now actually attach it

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenVertexArrays(1, &quadVAO);
    GL_Renderer::Instance()->fillBufferData(quadVAO, _quadVertices);
    GL_Renderer::Instance()->bindVertexAttrib(0, AttribType::VEC3, 5, 0);
    GL_Renderer::Instance()->bindVertexAttrib(1, AttribType::VEC2, 5, 3);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _textureColorbuffer);
    _screenShader.updateUniformInt("screenTexture", 2);
}


void PostProcessedScene::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //DefaultScene::render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(quadVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glUseProgram(_screenShader.shaderProgramId);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _textureColorbuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

PostProcessedScene::~PostProcessedScene()
{
}
