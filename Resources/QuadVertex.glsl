#version 400 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

out VS_OUT
{
	vec2 UV;
} 
vsOutput;

void main(void)
{
   gl_Position = vec4(pos, 1.f, 1.f);
   vsOutput.UV = uv;
}