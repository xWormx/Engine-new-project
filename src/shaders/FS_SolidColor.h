#version 330 core

in vec4 VS_col_out;
out vec4 solid_color;

void main()
{
	solid_color = VS_col_out; 
}