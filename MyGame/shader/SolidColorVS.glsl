
#version 330 core
layout (location = 0) in vec3 inPos;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectMat;

void main()
{
    gl_Position = projectMat * viewMat * modelMat * vec4(inPos, 1.0f);
}