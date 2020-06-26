#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in float Type;

uniform sampler2D Texture;

float getIntencityBasedOnNormal();
vec2 getTypePosition();

void main()
{
   float intencity = getIntencityBasedOnNormal();
   
   vec2 tileOffset = getTypePosition();
   vec2 tileCoord = (vec2(mod(TexCoords.x,1), mod(TexCoords.y,1))+tileOffset)/16;

   FragColor = vec4(texture(Texture, tileCoord).rgb*intencity, 1.0f);
}

vec2 getTypePosition(){
   if(Type <= 0)
       return vec2(0,0);
   if(Type <= 1)
       return vec2(2,1);
   if(Type <= 2)
      return vec2(0,9);
}

float getIntencityBasedOnNormal(){
    if(Normal.y > 0.9f)
        return 1.0f;
    if(Normal.y < -0.5f)
      return 0.3f;
    return 0.5f;
}