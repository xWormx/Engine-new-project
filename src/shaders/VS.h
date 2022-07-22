#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 col;
layout (location = 2) in vec2 texCoord;

out vec4 VS_col_out;
out vec2 VS_texCoord_out;

uniform vec2 winDim;
uniform vec2 ballPos;

void main()
{
    
	vec4 v4_pos;
	v4_pos.x = pos.x - 1.0f;
	v4_pos.y = pos.y - 1.0f;
	v4_pos.z = pos.z;
	v4_pos.w = 1.0f;
	
	gl_Position = v4_pos;
	VS_col_out = col;
	VS_texCoord_out = texCoord;
    
}
