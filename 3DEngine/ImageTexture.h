#pragma once
#include <glad/glad.h>
#include <iostream>
#include "stb_image/stb_image.h"

class ImageTexture
{
public:
    std::string type;

    ImageTexture(const char *name, std::string type);
    void Enable(const unsigned int uniteId);

private:
    unsigned int _textureId;
};

