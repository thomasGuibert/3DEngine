#pragma once
#include <map>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <glad/glad.h>

#include <Shader.h>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class TextRenderer
{
public:
    TextRenderer();
    ~TextRenderer();
    void renderText(std::string text, float x, float y, float scale, glm::vec3 color);
private:
    unsigned int _VAO, _VBO;
    Shader _shader;
};

