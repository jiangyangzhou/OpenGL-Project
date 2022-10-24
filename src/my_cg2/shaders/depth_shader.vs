#version 330 core
layout (location = 0) in vec3 position;
#define NR_POINT_LIGHTS 3

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position =  lightSpaceMatrix * model * vec4(position, 1.0f);
}