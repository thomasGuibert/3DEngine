#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;

    FragColor = vec4(ambient, 1.0f) * texture(texture1, TexCoord);
}
