#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube cubeTexture;
float ShadowCalculation(vec4 fragPosLightSpace);
void main()
{    
    FragColor = texture(cubeTexture, TexCoords);
}

