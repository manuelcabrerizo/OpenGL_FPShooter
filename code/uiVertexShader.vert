#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 View;
uniform mat4 Proj;
uniform mat4 World;

out vec3 ourColor;

void main()
{
    gl_Position = Proj * World * vec4(aPos, 1.0);
    ourColor = aColor;
}   
