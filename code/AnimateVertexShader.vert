#version 330 core
const int MAX_JOINT = 256;
const int MAX_WEIGHTS = 3;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 aTextCoord;
layout (location = 3) in ivec3 in_jointIndices;
layout (location = 4) in vec3 in_weights;

uniform mat4 View;
uniform mat4 Proj;
uniform mat4 World;
uniform float jointTransforms[MAX_JOINT];

out vec2 ourColor;
out vec3 normal;

mat4 GetMat4(int indice, float array[MAX_JOINT])
{
    mat4 result;
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            result[y][x] = array[((y*4)+x) + indice*16];
        }
    }
    return result;
}

void main()
{
	vec4 totalLocalPos = vec4(0.0);
	vec4 totalNormal = vec4(0.0);
	for(int i=0;i<MAX_WEIGHTS;i++)
    {
        if(in_jointIndices[i] != -1)
        {
		    mat4 jointTransform = GetMat4(in_jointIndices[i], jointTransforms);
		    vec4 posePosition = jointTransform * vec4(in_position, 1.0);
		    totalLocalPos += posePosition * in_weights[i];
		
		    //vec4 worldNormal = jointTransform * vec4(in_normal, 0.0);
		    //totalNormal += worldNormal * in_weights[i];
        }
	}
    
    gl_Position = Proj * View * World * totalLocalPos;
    normal = in_normal.xyz;
    ourColor = aTextCoord;
}  
