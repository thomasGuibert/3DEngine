#define STB_IMAGE_IMPLEMENTATION
#include "ImageTexture.h"

ImageTexture::ImageTexture(unsigned int width, unsigned int height) : 
    _width(width), 
    _height(height), 
    _data2(std::vector<float>(width*height * 4, 0.5))
{
    glGenTextures(1, &_textureId);
}

void ImageTexture::setPixel(glm::vec2 position, glm::vec4 color)
{
    int row = position.x;
    int col = position.y;

    int pos = row * _width * 4 + col * 4;
    _data2[pos] = color.r;
    _data2[pos + 1] = color.g;
    _data2[pos + 2] = color.b;
    _data2[pos + 3] = color.a;
}

void ImageTexture::buildImage() {
    glBindTexture(GL_TEXTURE_2D, _textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_FLOAT, _data2.data());
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_FLOAT, _data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

ImageTexture::ImageTexture(const char *name, std::string type) {
    this->type = type;

    glGenTextures(1, &_textureId);

    int width, height, nrComponents;
    _data = stbi_load(name, &width, &height, &nrComponents, 0);
    if (_data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, _textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, _data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(_data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << name << std::endl;
        stbi_image_free(_data);
    }
};

void ImageTexture::Enable(const unsigned int uniteId) {
    switch (uniteId)
    {
    case 0:
        glActiveTexture(GL_TEXTURE0);
        break;
    case 1:
        glActiveTexture(GL_TEXTURE1);
        break;
    case 2:
        glActiveTexture(GL_TEXTURE2);
        break;
    default:
        break;
    }
    glBindTexture(GL_TEXTURE_2D, _textureId);
};
