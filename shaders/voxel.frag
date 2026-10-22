#version 330 core

in vec3 vPos;
out vec4 FragColor;

void main()
{
    vec3 color = vPos + vec3(0.5);
    FragColor = vec4(color, 1.0);
}