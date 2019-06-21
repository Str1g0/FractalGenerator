#version 330 core

in vec2 UV;
in vec3 NRM;

out vec4 FragmentColor;
//uniform sampler2D text;

void main()
{
	FragmentColor = vec4(1.0, 0.2, 0.3, 1.0f);
}