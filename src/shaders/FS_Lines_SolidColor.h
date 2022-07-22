#version 330 core

in vec4 VS_out_col;


out vec4 FS_out_col;



void main()
{

	vec4 color = VS_out_col;

	FS_out_col = color;
}
