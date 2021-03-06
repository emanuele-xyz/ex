#version 430 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 vertTexCoord;

out vec2 fragTexCoord;

void main()
{
    gl_Position = vec4(vertPos, 1.0);

    fragTexCoord = vertTexCoord;
}
