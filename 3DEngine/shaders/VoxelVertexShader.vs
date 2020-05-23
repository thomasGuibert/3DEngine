#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormals;
layout (location = 3) in float aType;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out vec3 Normal;
out float Type;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    TexCoords = vec2(aTexCoords.x, aTexCoords.y);
    Normal = normalize(aNormals);
    Type = aType;
}

