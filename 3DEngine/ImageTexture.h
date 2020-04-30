#pragma once
#include <glad/glad.h>
#include <iostream>
#include "stb_image/stb_image.h"

class ImageTexture
{
public:
    ImageTexture(const char *name);
    void Enable(const unsigned int uniteId);

private:
    unsigned int _textureId;
};

