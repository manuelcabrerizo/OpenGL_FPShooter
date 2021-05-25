#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;
out float visibility;

uniform mat4 Proj;
uniform mat4 View;
uniform mat4 World;

const float density = 0.3;
const float gradient = 1.5;

void main()
{
    vec4 vertPosRelativeToCamera = View * World * vec4(aPos, 1.0);
    float dist = length(vertPosRelativeToCamera.xyz);
    visibility = exp(-pow((dist * density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);

    TexCoords = vec3(aPos.x, aPos.y, aPos.z);
    gl_Position = Proj * View * vec4(aPos, 1.0);
} 
