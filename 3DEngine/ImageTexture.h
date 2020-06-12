#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "stb_image/stb_image.h"

class ImageTexture
{
public:
    std::string type;

    ImageTexture(unsigned int width, unsigned int height);
    ImageTexture(const char *name, std::string type);
    void setPixel(glm::vec2 position, glm::vec4 color);
    void buildImage();
    void Enable(const unsigned int uniteId);

private:
    unsigned int _width;
    unsigned int _height;
    unsigned char* _data;
    std::vector<float> _data2;
    unsigned int _textureId;
};

