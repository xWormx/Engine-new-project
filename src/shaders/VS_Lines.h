#version 330 core

layout (location = 3) in vec2 VS_in_pos;
layout (location = 4) in vec4 VS_in_col;


out vec4 VS_out_col;


void main()
{

	vec4 pos;
	pos.x = VS_in_pos.x - 1.0f;
	pos.y = VS_in_pos.y - 1.0f;
	pos.z = 0.0f;
	pos.w = 1.0f;

	gl_Position = pos;

	vec4 color = VS_in_col;

	VS_out_col = color;
}
