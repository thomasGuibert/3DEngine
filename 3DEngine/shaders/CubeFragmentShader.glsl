#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 normal = normalize(Normal);

    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDirection = normalize(lightPos - FragPos);
    float diffuse = max(dot(normal,lightDirection), 0.0);

    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflection = reflect(-lightDirection, normal);
    vec3 specular = pow(max(dot(viewDir, reflection), 0.0), 32) * specularStrength * lightColor;

    FragColor = (vec4(ambient, 1.0f) + diffuse + vec4(specular, 1.0f)) * texture(texture1, TexCoord);
}