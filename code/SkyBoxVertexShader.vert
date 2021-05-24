#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 Proj;
uniform mat4 View;


void main()
{
    TexCoords = vec3(aPos.x, aPos.y, aPos.z);
    gl_Position = Proj * View * vec4(aPos, 1.0);
} 
