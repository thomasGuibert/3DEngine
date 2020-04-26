#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    vec3 normal = normalize(Normal);

    vec3 ambient  = light.ambient * material.ambient;

    vec3 lightDirection = normalize(lightPos - FragPos);
    float diff = max(dot(normal,lightDirection), 0.0);
    vec3 diffuse  = light.diffuse * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflection), 0.0), 32) * material.shininess;
    vec3 specular = light.specular * (spec * material.specular);  

    FragColor = (vec4(ambient, 1.0f) + vec4(diffuse, 1.0f) + vec4(specular, 1.0f)) * texture(texture1, TexCoord);
}