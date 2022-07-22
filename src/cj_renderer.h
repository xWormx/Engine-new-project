#ifndef CJ_RENDER_H
#define CJ_RENDER_H

#define LOAD_IMAGE 0
#define LOAD_FONT 1

#define QUAD_IMAGE 0
#define QUAD_TEXT_STATIC 1
#define QUAD_TEXT_DYNAMIC 2
#define QUAD_NOT_INITIALIZED 0
#define QUAD_INITIALIZED 1

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"


#include "cj_shader.h"

struct TEXTUREINFO
{
    
    	u32 texture_id;
    	
    	// for loading font textures
    	stbtt_packedchar packed_char[126];
    	stbtt_pack_context pack_context;
    	stbtt_aligned_quad glyph_quad;
	u32 glyph_size;
    	
    	float xp;
    	float yp;
    	
    	u32 atlas_w = 1024;
    	u32 atlas_h = 1024;
    	
    	unsigned char *font_file_data;
    	unsigned char *pixels;

	u32 texture_loaded;
};

struct CJ_VTX_COL_LINE
{
	V2f p;
	V4f col;
};

struct CJ_VTX_SOLCOL_QUAD
{
	V2f pos;
	V4f col;
};

struct CJ_VTX_SPRITE
{
	V3f pos;
	V4f col;
	V2f tCoord;
};

struct CJ_VTX_ATTRIB
{
	u32 size;
	GLenum type;
	u32 stride;
	u32 offset;
};

struct CJ_VBO
{
	u32 id;
	u8 *base;
	u32 used;
	u32 size;
};

struct CJ_IBO
{
	u32 id;
	u32 *base;
	u32 used;
	u32 size;
};

struct CJ_RENDER_COMMANDS
{
		u32 n_entities_to_draw;
		u32 texture_id;
		u32 draw_offset[512];
};

struct CJ_RENDERER
{
	CJ_RENDER_COMMANDS commands;

	u32 VAO_sprite_quad;
	u32 VAO_font;
	u32 VAO_lines;
	u32 VAO_solcol_quad;

	CJ_VBO vbo_lines;
	u32 max_lines_to_render;

	CJ_VBO vbo_solcol_quad;
	u32 max_solcol_quads_to_render;

	CJ_VBO vbo_sprite;
	u32 max_sprites_to_render;

	CJ_VBO vbo_text;
	u32 n_chars_drawn;
	u32 max_chars_to_render;
	u32 vbo_text_index_tracker;

	CJ_IBO ibo_sprite;

	SHADER_PROGRAM shader[10];

	TEXTUREINFO texture_batch[10];
};

void InitializeVBO(CJ_VBO *vbo, u8* base, u32 size, u32 n_attribs, CJ_VTX_ATTRIB *vtx_attrib);
CJ_RENDERER CreateRenderer();
void UpdateRenderer(CJ_RENDERER *renderer);
void InvertSTBBuffer(unsigned char *memory, u32 w, u32 h);
void LoadTexture(TEXTUREINFO *tex_i, char *texture_path, u32 texture_type, i32 glyph_size);

void RenderClear(V4f col);

void DrawArrays(CJ_RENDERER *renderer, u32 shader_index, u32 tex_index, u32 n_ents_to_draw);
void DrawArraysLines(CJ_RENDERER *renderer, u32 shader_index, u32 n_lines_to_draw);
void DrawElements(CJ_RENDERER *renderer, u32 shader_index, u32 tex_index, u32 n_ents_to_draw, u32 n_ents_offset);

void InitializeVBO(CJ_VBO *vbo, u8* base, u32 size, u32 n_attribs, CJ_VTX_ATTRIB *vtx_attrib)
{
	vbo->base = base;
	vbo->size = size;

	glGenBuffers(1, &vbo->id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
	glBufferData(GL_ARRAY_BUFFER, vbo->size, vbo->base, GL_DYNAMIC_DRAW);

	CJ_VTX_ATTRIB *attrib = (CJ_VTX_ATTRIB*)malloc(n_attribs * sizeof(CJ_VTX_ATTRIB));

	for(u32 i = 0; i < n_attribs; i++)
	{
		attrib[i] = vtx_attrib[i];

		attrib[i].offset *= sizeof(float);
		attrib[i].stride *= sizeof(float);
		glEnableVertexAttribArray(i);	glVertexAttribPointer(i, attrib[i].size, attrib[i].type, false, attrib[i].stride, (void*)attrib[i].offset);
	}

	free(attrib);

}

CJ_RENDERER CreateRenderer()
{

	CJ_RENDERER renderer = {};

	glGenVertexArrays(1, &renderer.VAO_lines);
	glBindVertexArray(renderer.VAO_lines);

	glGenVertexArrays(1, &renderer.VAO_solcol_quad);
	glBindVertexArray(renderer.VAO_solcol_quad);

	glGenVertexArrays(1, &renderer.VAO_font);
	glBindVertexArray(renderer.VAO_font);

	glGenVertexArrays(1, &renderer.VAO_sprite_quad);
	glBindVertexArray(renderer.VAO_sprite_quad);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	u32 sol_col 		= 0;
	u32 textured_quad 	= 1;
	u32 font		= 2;
	u32 line		= 3;


	CreateShader(&renderer.shader[sol_col], "..\\shader\\VS.h", GL_VERTEX_SHADER);
	CreateShader(&renderer.shader[sol_col], "..\\shader\\FS_solid_col.h", GL_FRAGMENT_SHADER);
	LinkShaderProgram(&renderer.shader[sol_col]);
	UseShaderProgram(&renderer.shader[sol_col]);


	CreateShader(&renderer.shader[textured_quad], "..\\shader\\VS.h", GL_VERTEX_SHADER);
	CreateShader(&renderer.shader[textured_quad], "..\\shader\\FS_Texture.h", GL_FRAGMENT_SHADER);
	LinkShaderProgram(&renderer.shader[textured_quad]);
	UseShaderProgram(&renderer.shader[textured_quad]);

	CreateShader(&renderer.shader[font], "..\\shader\\VS_text.h", GL_VERTEX_SHADER);
	CreateShader(&renderer.shader[font], "..\\shader\\FS_Font.h", GL_FRAGMENT_SHADER);
	LinkShaderProgram(&renderer.shader[font]);
	UseShaderProgram(&renderer.shader[font]);

	CreateShader(&renderer.shader[line], "..\\shader\\VS_Lines.h", GL_VERTEX_SHADER);
	CreateShader(&renderer.shader[line], "..\\shader\\FS_solid_col.h", GL_FRAGMENT_SHADER);
	LinkShaderProgram(&renderer.shader[line]);
	UseShaderProgram(&renderer.shader[line]);



	CJ_VTX_ATTRIB vbo_lines_attrib[2];
	vbo_lines_attrib[0].size 	= 2;
	vbo_lines_attrib[0].type 	= GL_FLOAT;
	vbo_lines_attrib[0].stride 	= 6;
	vbo_lines_attrib[0].offset 	= 0;

	vbo_lines_attrib[1].size 	= 4;
	vbo_lines_attrib[1].type 	= GL_FLOAT;
	vbo_lines_attrib[1].stride 	= 6;
	vbo_lines_attrib[1].offset 	= 2;

	u32 verts_per_line 		= 2;
	u32 size_per_vertex 		= sizeof(CJ_VTX_COL_LINE); 
	u32 max_lines			= 100;
	u32 size 			= max_lines * verts_per_line * size_per_vertex;
	u8 *vertex_memory 		= (u8*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	glBindVertexArray(renderer.VAO_lines);
	InitializeVBO(&renderer.vbo_lines, vertex_memory, size, ArrayCount(vbo_lines_attrib), vbo_lines_attrib);

	renderer.max_lines_to_render = max_lines;


	u32 verts_per_quad 		= 6;
	size_per_vertex 		= sizeof(CJ_VTX_SOLCOL_QUAD); 
	u32 max_quads			= 100;
	size 				= max_quads * verts_per_quad * size_per_vertex;
	vertex_memory 			= (u8*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	// Can reuse attribs from lines since the layout is same for solcol_quads as for lines.
	glBindVertexArray(renderer.VAO_solcol_quad);
	InitializeVBO(&renderer.vbo_solcol_quad, vertex_memory, size, ArrayCount(vbo_lines_attrib), vbo_lines_attrib);

	renderer.max_solcol_quads_to_render = max_quads;

	CJ_VTX_ATTRIB vbo_sprite_attrib[3];
	vbo_sprite_attrib[0].size 	= 3;
	vbo_sprite_attrib[0].type 	= GL_FLOAT;
	vbo_sprite_attrib[0].stride 	= 9;
	vbo_sprite_attrib[0].offset 	= 0;

	vbo_sprite_attrib[1].size 	= 4;
	vbo_sprite_attrib[1].type 	= GL_FLOAT;
	vbo_sprite_attrib[1].stride 	= 9;
	vbo_sprite_attrib[1].offset 	= 3;

	vbo_sprite_attrib[2].size 	= 2;
	vbo_sprite_attrib[2].type 	= GL_FLOAT;
	vbo_sprite_attrib[2].stride 	= 9;
	vbo_sprite_attrib[2].offset 	= 7;

	verts_per_quad 			= 4;
	size_per_vertex 		= sizeof(CJ_VTX_SPRITE); 
	max_quads			= 5000;
	size 				= max_quads * verts_per_quad * size_per_vertex;
	vertex_memory 			= (u8*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	glBindVertexArray(renderer.VAO_sprite_quad);
	InitializeVBO(&renderer.vbo_sprite, vertex_memory, size, ArrayCount(vbo_sprite_attrib), vbo_sprite_attrib);

	renderer.max_sprites_to_render = max_quads;

	u32 IBO 		  = 0;
	u32 quads 		  = size / (sizeof(CJ_VTX_SPRITE) * 4);
	u32 ibo_size 		  = quads * 6  * sizeof(u32);
	u32 *ibo_data 		  = (u32*)VirtualAlloc(0, ibo_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	renderer.ibo_sprite.base  = ibo_data;
	renderer.ibo_sprite.size  = ibo_size;

	for(u32 i = 0; i < quads; i++)
	{
		ibo_data[0 + (i * 6)] = 0 + (i * 4);
		ibo_data[1 + (i * 6)] = 1 + (i * 4);
		ibo_data[2 + (i * 6)] = 2 + (i * 4);
		ibo_data[3 + (i * 6)] = 0 + (i * 4);
		ibo_data[4 + (i * 6)] = 2 + (i * 4);
		ibo_data[5 + (i * 6)] = 3 + (i * 4);
	}

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_size, ibo_data, GL_DYNAMIC_DRAW);

	CJ_VTX_ATTRIB vbo_text_attrib[3];
	vbo_text_attrib[0].size 	= 3;
	vbo_text_attrib[0].type 	= GL_FLOAT;
	vbo_text_attrib[0].stride 	= 9;
	vbo_text_attrib[0].offset 	= 0;

	vbo_text_attrib[1].size 	= 4;
	vbo_text_attrib[1].type 	= GL_FLOAT;
	vbo_text_attrib[1].stride 	= 9;
	vbo_text_attrib[1].offset 	= 3;

	vbo_text_attrib[2].size 	= 2;
	vbo_text_attrib[2].type 	= GL_FLOAT;
	vbo_text_attrib[2].stride 	= 9;
	vbo_text_attrib[2].offset 	= 7;


	u32 max_chars 			= 1024 * 20;
	u32 vertices_per_quad 		= 6; // since we're not using IBO for this, else it would be 4
	u32 size_text 			= max_chars * vertices_per_quad * sizeof(CJ_VTX_SPRITE);
	u8 *vertex_memory_for_text 	= (u8*)VirtualAlloc(0, size_text, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	renderer.max_chars_to_render 	= max_chars;

	glBindVertexArray(renderer.VAO_font);
	InitializeVBO(&renderer.vbo_text, vertex_memory_for_text, size_text, ArrayCount(vbo_text_attrib), vbo_text_attrib);

	LoadTexture(&renderer.texture_batch[0], "assets\\tileSheet32x32.png", LOAD_IMAGE, 0);
	LoadTexture(&renderer.texture_batch[1], "C:\\windows\\fonts\\cour.ttf", LOAD_FONT, 64);
	LoadTexture(&renderer.texture_batch[2], "C:\\windows\\fonts\\candara.ttf", LOAD_FONT, 32);
	LoadTexture(&renderer.texture_batch[3], "..\\Fonts\\classicfont.TTF", LOAD_FONT, 24);

	return renderer;
}


#if 0
enum RENDER_ELEMENT_TYPE
{
		render_element_clear, render_element_viewport
};

struct RENDER_ELEMENT
{
		RENDER_ELEMENT_TYPE type;
		u32 size;
		u8 *data:
};

struct RENDER_ELEMENT_TEXTURED_QUAD
{
		u32 shader;
		u32 texture;
		u32 quads_to_draw;
		u32 quad_offset;
};

struct RENDER_COMMAND_BUFFER
{
	RENDER_ELEMENT *render_element_batch;
};

#endif

void UpdateRenderer(CJ_RENDERER *renderer, CJ_PLATFORM *platform)
{
        
#if 0
		RENDER_COMMAND_BUFFER *rcb;
	   	rcb->render_element_batch = malloc(1000 * sizeof(RENDER_ELEMENT);


		game->player->renderable = PushRenderable(renderer, render_element_sprite_quad);

		RENDER_COMMAND_BUFFER *render_command_buffer = rcb;

		while(render_command_buffer->render_element_batch)
		{
				RENDER_COMMAND_ELEMENT element = render_command_buffer->render_element_batch;
				switch(element.type)
				{
						case render_element_viewport:
								{
									V4i vpr = *(V4i*)element->data;
									glViewport(vpr.x, vpr.y, vpr.w, vpr.h);
									render_command_buffer->render_element_batch += element.size;
								} break;
						case render_element_clear:
								{
									RenderClear(element->clear_color);
									render_command_buffer->render_element_batch += sizeof(render_element_clear);
								} break;
						case render_element_textured_quad:
								{
									glBindVertexArray(renderer->VAO_sprite_quad);
									glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo_sprite.id);
									glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->vbo_sprite.size, (void*)renderer->vbo_sprite.base);

									RENDER_ELEMENT_TEXTURED_QUAD element = element.type;
									DrawElements(renderer, element->shader, element->texture, element->quads_to_draw, element->quad_offset);
								} break;
				}


		}
#endif

		glViewport(0, 0, platform->win_w, platform->win_h);
		RenderClear(v4f(0.011f, 0.011f, 0.111f, 1.0f));

		// RENDER SPRITES
		glBindVertexArray(renderer->VAO_sprite_quad);
		glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo_sprite.id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->vbo_sprite.size, (void*)renderer->vbo_sprite.base);
		DrawElements(renderer, 1, renderer->commands.texture_id, renderer->max_sprites_to_render, 0);
		
		// RENDER SOLCOL QUADS
		CJ_VTX_SOLCOL_QUAD *v = (CJ_VTX_SOLCOL_QUAD*)renderer->vbo_solcol_quad.base;
		glBindVertexArray(renderer->VAO_solcol_quad);
		glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo_solcol_quad.id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->vbo_solcol_quad.size, (void*)renderer->vbo_solcol_quad.base);
		DrawArrays(renderer, 0, 0, renderer->max_solcol_quads_to_render);
		
		// RENDER LINES //
		glBindVertexArray(renderer->VAO_lines);
		glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo_lines.id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->vbo_lines.size, (void*)renderer->vbo_lines.base);
		DrawArraysLines(renderer, 0, renderer->max_lines_to_render);
        	
		// RENDERING TEXT //
		// Test for using Uniform and maniuplating color via shader. OBS! Must UseShaderProgram first
		UseShaderProgram(&renderer->shader[2]);
		int posloc = glGetUniformLocation(renderer->shader[2].shaderProgram, "fpos");
		static V2f pc = v2f(0.01f, 0.01f);
		if(KeyPressed(platform, GLFW_KEY_LEFT))
		{
			pc.x -= 0.01f;
			pc.y -= 0.01f;
			printf("pc.x = %f\n", pc.x);
		}
		if(KeyPressed(platform, GLFW_KEY_RIGHT))
		{
			pc.x += 0.01f;
			pc.y += 0.01f;
			printf("pc.x = %f\n", pc.x);
		}
		pc.x = platform->cursor_pos.x;
		pc.y = platform->cursor_pos.y;
		pc.x = platform->cursor_pos_int.x;
		pc.y = platform->cursor_pos_int.y;
		glUniform2f(posloc, pc.x, pc.y);
		glBindVertexArray(renderer->VAO_font);
		glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo_text.id);
		// offset means offest into the VBO ont the GPU (vbo_text) not the buffer on the CPU
		glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->vbo_text.size, (void*)renderer->vbo_text.base);
		DrawArrays(renderer, 2, 3, renderer->max_chars_to_render);



		// I think this resets the dynamic text buffer and tracker.
		renderer->vbo_text_index_tracker = 0;
		renderer->n_chars_drawn		= 0;
		memset(renderer->vbo_text.base, 0, renderer->vbo_text.size);

}


void InvertSTBBuffer(unsigned char *memory, u32 w, u32 h)
{
    for(i32 y = 0; y < h / 2; y++)
    {
        for(i32 x = 0; x < w; x++)
        {
            i32 temp = memory[x + (y * w)];
            memory[x + (y * w)] = memory[x + ((h - 1) * w) - (y * w)];
            memory[x + ((h - 1) * w) - (y * w)] = temp;
        }
    }
}

void LoadTexture(TEXTUREINFO *tex_i, char *texture_path, u32 texture_type, i32 glyph_size)
{
	if(texture_type == LOAD_IMAGE) 
	{
	    
	    // Generating/binding and loading texture
	    i32 image_w = 0;
	    i32 image_h = 0;
	    i32 image_nChannels = 0;

	    if(tex_i->texture_id != 0)
	    {
		    glDeleteTextures(1, &tex_i->texture_id);
	    }
	    
	    glGenTextures(1, &tex_i->texture_id);
	    glBindTexture(GL_TEXTURE_2D, tex_i->texture_id);
	    
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	    
	    stbi_set_flip_vertically_on_load(true);
	    u8 *texture = stbi_load(texture_path, &image_w, &image_h, &image_nChannels, 0);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	    stbi_image_free(texture);
	    // End of gen/bind/load texture
	    
	    tex_i->texture_loaded = true;
	
	
	    
	} 
	else if (texture_type == LOAD_FONT)
	{
	    
	    if(tex_i->texture_id != 0)
	    {
		    glDeleteTextures(1, &tex_i->texture_id);
	    }

		tex_i->glyph_size = glyph_size;

	    // Start gen/bind/load font texture atlas
	    glGenTextures(1, &tex_i->texture_id);
	    glBindTexture(GL_TEXTURE_2D, tex_i->texture_id);
	    
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	    
	    size_t bytes_read = 0;
	    u32 fontf_sz = 4 * tex_i->atlas_w * tex_i->atlas_h;
	    tex_i->font_file_data = (unsigned char*)calloc(fontf_sz, sizeof(unsigned char));
	    tex_i->pixels = (unsigned char*)calloc(fontf_sz, sizeof(unsigned char));
	    
	    bytes_read = fread(tex_i->font_file_data, 1, fontf_sz, fopen(texture_path, "rb"));
	    
	    // init pixel_bitmap and packing context
	    bool success = 0;
	    success = stbtt_PackBegin(&tex_i->pack_context, tex_i->pixels, tex_i->atlas_w, tex_i->atlas_h, 0, 1, 0);
	    
	    // set detail level of characters in font which requires the pixel_bitmap to be bigger
	    stbtt_PackSetOversampling(&tex_i->pack_context, 2, 2);
	    
	    // Set what characters in the pixel_map you want STBTT_POINT_SIZE makes the charcter size in tex_i->pixels
	    stbtt_PackFontRange(&tex_i->pack_context, tex_i->font_file_data, 0, STBTT_POINT_SIZE(glyph_size), ' ', 126,  tex_i->packed_char);
	    
	    // Invert font texture becuase stb give's it to us upside down ( y increases downward )
	    InvertSTBBuffer(tex_i->pixels, tex_i->atlas_w,  tex_i->atlas_h);
	    
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, tex_i->atlas_w, tex_i->atlas_h, 0, GL_RED, GL_UNSIGNED_BYTE, tex_i->pixels);
	    
	    free(tex_i->pixels);
	    free(tex_i->font_file_data);
	    stbtt_PackEnd(&tex_i->pack_context);
	    // End the packing context and free font_tex_ile and font atlas pixels
	    
	    tex_i->texture_loaded = true;
	}
    
}

void RenderClear(V4f col)
{
	glClearColor(col.x, col.y, col.z, col.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void DrawArraysLines(CJ_RENDERER *renderer, u32 shader_index, u32 n_lines_to_draw)
{
		glLineWidth(2.0f);
		UseShaderProgram(&renderer->shader[shader_index]);
		glDrawArrays(GL_LINES, 2 * 0, n_lines_to_draw * 2);
}

void DrawArrays(CJ_RENDERER *renderer, u32 shader_index, u32 tex_index, u32 n_ents_to_draw)
{
		UseShaderProgram(&renderer->shader[shader_index]);
		glBindTexture(GL_TEXTURE_2D, renderer->texture_batch[tex_index].texture_id);
		glDrawArrays(GL_TRIANGLES, 0, n_ents_to_draw * 6);
}

void DrawElements(CJ_RENDERER *renderer, u32 shader_index, u32 tex_index, u32 n_ents_to_draw, u32 n_ents_offset)
{

	UseShaderProgram(&renderer->shader[shader_index]);
	glBindTexture(GL_TEXTURE_2D, renderer->texture_batch[tex_index].texture_id);
	glDrawElements(GL_TRIANGLES, 6 * n_ents_to_draw, GL_UNSIGNED_INT, (const void*)(n_ents_offset * 6 * sizeof(u32))); 

}

#endif
