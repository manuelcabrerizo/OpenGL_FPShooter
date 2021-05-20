#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoord;

uniform mat4 View;
uniform mat4 Proj;
uniform mat4 World;

out vec2 ourColor;
out vec3 normal;

void main()
{
    gl_Position = Proj * View * World * vec4(aPos, 1.0);
    normal = mat3(transpose(inverse(World))) * aNormal;
    ourColor = aTextCoord;
}   
