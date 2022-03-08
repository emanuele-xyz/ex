#version 430 core

in vec2 fragTextureCoordinates;

out vec4 outColor;

uniform sampler2D texture0;

void main()
{
    outColor = texture(texture0, fragTextureCoordinates);
}
