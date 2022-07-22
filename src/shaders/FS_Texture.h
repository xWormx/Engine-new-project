#version 330 core

in vec4 VS_col_out;
in vec2 VS_texCoord_out;

out vec4 col_out;

uniform sampler2D myTexture;

void main()
{
	vec4 vertex_col = VS_col_out;
	col_out = texture(myTexture, VS_texCoord_out) * vertex_col;
}
