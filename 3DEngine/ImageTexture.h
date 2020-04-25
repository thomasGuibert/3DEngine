#pragma once
#include <glad/glad.h>
#include <iostream>
#include "stb_image/stb_image.h"

class ImageTexture
{
public:
    ImageTexture(const char *name);
    void Enable();

private:
    unsigned int _textureId;
};

