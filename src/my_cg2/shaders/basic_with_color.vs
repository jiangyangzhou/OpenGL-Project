#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; 
layout (location = 2) in vec4 aColors;
#define NR_POINT_LIGHTS 2

out vec3 FragPos;
out vec3 Normal;
out vec4 Color;
out vec4 FragPosLightSpace[NR_POINT_LIGHTS];

uniform mat4 model; 
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix[NR_POINT_LIGHTS];

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    Color = aColors;
    for(int i=0; i<NR_POINT_LIGHTS; i++){
        FragPosLightSpace[i] = lightSpaceMatrix[i] * vec4(FragPos, 1.0);
    }
    gl_Position = projection * view * vec4(FragPos, 1.0);
}