#version 330 core
out vec4 FragColor;

in vec3 TexCoords;
in float visibility;

uniform samplerCube skybox;

void main()
{
    vec3 fogColor = vec3(0.4, 0.5, 0.4);

    FragColor = texture(skybox, TexCoords);
    FragColor = mix(vec4(fogColor, 1.0), FragColor, visibility);

}
