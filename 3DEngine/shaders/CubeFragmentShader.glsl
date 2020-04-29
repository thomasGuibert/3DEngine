#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform vec3 globalLightDirection;

void main()
{
    vec3 normal = normalize(Normal);
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                                light.quadratic * (distance * distance)); 

    vec3 lightDirection = normalize(-globalLightDirection);
    vec3 pointLightDirection = normalize(light.position - FragPos);

    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoord))*0.2;

    float pointLightDiff = max(dot(normal,pointLightDirection), 0.0);
    float lightDiff = max(dot(normal,lightDirection), 0.0);
    vec3 diffuse  = light.diffuse * (pointLightDiff * vec3(texture(material.diffuse, TexCoord)));
    diffuse *= attenuation;
    diffuse += lightDiff * vec3(texture(material.diffuse, TexCoord));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 pointLightReflection = reflect(-pointLightDirection, normal);
    vec3 reflection = reflect(-lightDirection, normal);
    float pointLightSpec = pow(max(dot(viewDir, pointLightReflection), 0.0), 32) * material.shininess;
    float lightSpec = pow(max(dot(viewDir, reflection), 0.0), 32) * material.shininess;

    vec3 specular = light.specular * (pointLightSpec * vec3(texture(material.specular, TexCoord)));  
    specular *= attenuation;
    specular += lightSpec * vec3(texture(material.specular, TexCoord));

    FragColor = (vec4(ambient, 1.0f) + vec4(diffuse, 1.0f) + vec4(specular, 1.0f));
}