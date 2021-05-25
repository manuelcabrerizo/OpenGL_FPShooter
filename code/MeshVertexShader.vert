#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoord;

uniform mat4 View;
uniform mat4 Proj;
uniform mat4 World;

out vec2 ourColor;
out vec3 normal;
out float visibility;

const float density = 0.06;
const float gradient = 1.5;

void main()
{
    vec4 vertPosRelativeToCamera = View * World * vec4(aPos, 1.0);
    float dist = length(vertPosRelativeToCamera.xyz);
    visibility = exp(-pow((dist * density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);

    gl_Position = Proj * View * World * vec4(aPos, 1.0);
    normal = mat3(transpose(inverse(World))) * aNormal;
    ourColor = aTextCoord;
}   
