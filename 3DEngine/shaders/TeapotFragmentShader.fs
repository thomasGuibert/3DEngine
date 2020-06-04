#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

in vec3 Normal;
in vec3 FragPos;
in vec3 ViewPos;

uniform Material material;
uniform DirectionalLight globalLight;
#define NR_POINT_LIGHTS 4
uniform PointLight light[NR_POINT_LIGHTS];

vec4 computeGlobalLight();
vec4 computePointLight(PointLight light);
void main()
{
    FragColor = vec4(0.0f);
    FragColor += computeGlobalLight();
    for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
        FragColor +=  computePointLight(light[i]) * 2.0f;
    }
    FragColor = vec4(FragColor.r, FragColor.g, FragColor.b, 0.5f);
}

vec4 computeGlobalLight(){
    vec3 normal = normalize(Normal);

    //Ambient
    vec3 ambient = globalLight.ambient * material.diffuse;

    //Diffuse
    vec3 lightDirection = normalize(-globalLight.direction);
    float lightDiff = max(dot(normal,lightDirection), 0.0f);
    vec3 diffuse  = lightDiff * globalLight.diffuse * material.diffuse;

    //Sepcular
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 lightReflection = reflect(-lightDirection, normal);
    float lightSpec = pow(max(dot(viewDir, lightReflection), 0.0f), material.shininess);
    vec3 specular = lightSpec * globalLight.specular * material.specular;

    return vec4(ambient + diffuse + specular, 0.5f);
}

vec4 computePointLight(PointLight light){
    vec3 normal              = normalize(Normal);
    vec3 lightDirection      = normalize(light.position - FragPos);
    float distance           = length(light.position - FragPos);
    float attenuation        = 1.0f / (light.constant + light.linear * distance + 
                                       light.quadratic * (distance * distance)); 
    //Ambient
    vec3 ambient  = 0.2 * light.ambient * material.diffuse;

    //Diffuse
    float lightDiff = max(dot(normal,lightDirection), 0.0f);
    vec3 diffuse  = lightDiff * attenuation * light.diffuse  * material.diffuse;

    //Specular
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 lightReflection = reflect(-lightDirection, normal);
    float lightSpec = pow(max(dot(viewDir, lightReflection), 0.0f), material.shininess);
    vec3 specular = lightSpec * attenuation * light.specular * material.specular;

    return vec4(ambient + diffuse + specular, 0.5f);
}