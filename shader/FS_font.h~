#version 330 core


in vec4 VS_out_col;
in vec2 VS_out_tCoord;


out vec4 font_surface;


uniform sampler2D font_texture;
uniform vec2 fpos;


void main()
{
	vec4 col = VS_out_col;
	vec4 fc = gl_FragCoord;

	//col.g -= fpos.x;
	//col.b -= fpos.y;
	//if (fc.x < (fpos.x + 10.0f) &&
	//    fc.y < (fpos.y + 10.0f) &&
	//    fc.x > (fpos.x - 10.0f) &&
	//    fc.y > (fpos.y - 10.0f))
	//{
	//	col = VS_out_col;
	//}
	//else
	//{
	//	col.r = 0.0f;
	//	col.g = 0.0f;
	//	col.b = 0.0f;
	//}

	//float radius = distance(fc.xy, fpos.xy);
	//if(radius < 100.0)
	//{
	//	float alpha = 1.0 - (radius / 100.0);
	//	col.a = alpha;
	//	col.g = alpha;
	//}
	//else
	//{
	//	col.a = 0.2f;
	//}


	font_surface = vec4(col.rgb, texture(font_texture, VS_out_tCoord).r);
	font_surface *= col.a;
}
