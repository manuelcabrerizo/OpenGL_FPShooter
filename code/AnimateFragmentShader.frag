#version 330 core

out vec4 FragColor;
in vec2 ourColor;
in vec3 normal;

uniform sampler2D texture1;

void main()
{
    vec3 lightDirection = vec3(0.0, 0.5, -1.0);
    vec3 norm = normalize(normal);
    float shadow = dot(norm, lightDirection);
    if(shadow <= 0.0)
    {
        shadow = 0.0;
    }
    vec3 objectColor = texture(texture1, ourColor).xyz;
    FragColor = (vec4(objectColor, 1.0) * (shadow + 0.3)) * 0.7;
}




