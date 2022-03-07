#version 430 core

in vec2 fragTexCoord;

out vec4 outColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    outColor = mix(texture(texture0, fragTexCoord), texture(texture1, fragTexCoord), 0.50);
}
