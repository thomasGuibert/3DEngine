#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D ourTexture;
uniform sampler2D Texture;

float getIntencityBasedOnNormal();

void main()
{
    FragColor = texture(ourTexture, TexCoords);
    float intencity = getIntencityBasedOnNormal();
    FragColor = vec4(texture(Texture, TexCoords).rgb*intencity, 1.0f);
}

float getIntencityBasedOnNormal(){
    if(Normal.y > 0.9f)
        return 1.0f;
    if(Normal.y < -0.5f)
      return 0.3f;
    return 0.5f;
}

