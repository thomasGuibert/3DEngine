#include "SkyBox.h"

Skybox::Skybox(const char * path, Camera& camera) : _camera(camera)
{
    loadSkyboxTexture(path);

    _shader = new Shader("shaders/SkyboxVertexShader.vs", "shaders/SkyboxFragmentShader.fs");
    _shader->updateUniformInt("skyboxText", 0);
    loadSkyboxVertices();
}

void Skybox::loadSkyboxVertices()
{
    glGenVertexArrays(1, &_skyboxVAO);
    GL_Renderer::Instance()->fillBufferData(_skyboxVAO, skyboxVertices);
    GL_Renderer::Instance()->bindVertexAttrib(0, AttribType::VEC3, 3, 0);
}

void Skybox::loadSkyboxTexture(std::string path)
{
    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < _imageFaces.size(); i++)
    {

        unsigned char *data = stbi_load((path + _imageFaces[i]).c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << _imageFaces[i].c_str() << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::render()
{
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    //_shader.use();
    glm::mat4 view = glm::mat4(glm::mat3(_camera.lookAt())); // remove translation from the view matrix
    _shader->updateUniformMat4("view", view);
    glm::mat4 projection = _camera.perspective();
    _shader->updateUniformMat4("projection", projection);
    // skybox cube
    glBindVertexArray(_skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

Skybox::~Skybox()
{
}
