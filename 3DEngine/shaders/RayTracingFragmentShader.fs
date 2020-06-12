#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D rayTracingTexture;

void main()
{
    FragColor = vec4(texture(rayTracingTexture, TexCoords));
} 