#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform samplerCube skybox;
uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight globalLight;
#define NR_POINT_LIGHTS 1  
uniform PointLight light[NR_POINT_LIGHTS];

vec4 computeGlobalLight();
vec4 computePointLight(PointLight light);
void main()
{
    //FragColor = vec4(0.0f);
    //FragColor += computeGlobalLight();
    //for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
    //    FragColor += computePointLight(light[i]) * 2.0f;
    //}
    vec3 I = normalize(FragPos - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}

vec4 computeGlobalLight(){
    vec3 normal = normalize(Normal);

    //Ambient
    vec3 ambient = globalLight.ambient * vec3(texture(material.diffuse, TexCoords));

    //Diffuse
    vec3 lightDirection = normalize(-globalLight.direction);
    float lightDiff = max(dot(normal,lightDirection), 0.0f);
    vec3 diffuse  = lightDiff * globalLight.diffuse * vec3(texture(material.diffuse, TexCoords));

    //Sepcular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightReflection = reflect(-lightDirection, normal);
    float lightSpec = pow(max(dot(viewDir, lightReflection), 0.0f), material.shininess);
    vec3 specular = lightSpec * globalLight.specular * vec3(texture(material.specular, TexCoords));

    return vec4(ambient + diffuse + specular, 1.0f);
}

vec4 computePointLight(PointLight light){
    vec3 normal = normalize(Normal);
    vec3 lightDirection      = normalize(light.position - FragPos);
    float distance           = length(light.position - FragPos);
    float attenuation        = 1.0f / (light.constant + light.linear * distance + 
                                       light.quadratic * (distance * distance)); 
    //Ambient
    vec3 ambient  = 0.2 * light.ambient * vec3(texture(material.diffuse, TexCoords));

    //Diffuse
    float lightDiff = max(dot(normal,lightDirection), 0.0f);
    vec3 diffuse  = lightDiff * attenuation * light.diffuse  * vec3(texture(material.diffuse, TexCoords));

    //Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightReflection = reflect(-lightDirection, normal);
    float lightSpec = pow(max(dot(viewDir, lightReflection), 0.0f), material.shininess);
    vec3 specular = lightSpec * attenuation * light.specular * vec3(texture(material.specular, TexCoords));

    return vec4(ambient + diffuse + specular, 1.0f);
}