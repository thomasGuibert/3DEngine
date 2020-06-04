#version 330 core

out vec4 FragColor;

struct Material {
    vec3 diffuse;
};

uniform Material material;

void main()
{
    FragColor = vec4(material.diffuse, 1.0f);
}
