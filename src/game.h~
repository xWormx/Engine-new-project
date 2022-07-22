// TODO (Calle): Se till at DrawText() kan returnera storleken på hela storleken på text ytan
// dvs en text som spänner över flera rader!
// TODO (Calle: Fixa en timer. 
// TODO (Calle: Ändra storleken på "penseln/cursorn" som målar sprites på kartan.


#ifndef CJ_GAME_H
#define CJ_GAME_H


struct TEXT_INFO
{
	//V2f sentence_pos;
	V3f sentence_pos;
	V2f sentence_size; // NOTE: for now this is the actual point relative to the window, like the pos more like "upper and rightmost"
	u32 n_files_in_dir;

	// This is for making the DrawText() Return n_rows number of collisionboxes for each sentence on a 
	// new row.
	u32 n_rows;
};

enum GAME_STATE
{
	game_state_main_menu, game_state_console
};

struct COLOR_LINE
{
	V2f p1;
	V2f p2;
	V4f col;
};


struct SOLCOL_QUAD
{
	V2f pos;
	V2f size;
	V4f col;
};

SOLCOL_QUAD StdSolColQuad(V2f p = v2f(0.01f, 0.1f),
		 	  V2f sz = v2f(0.1f, 0.1f),
		 	  V4f col = COLOR_WHITE)
{
	SOLCOL_QUAD sq = {};
	sq.pos = p;
	sq.size = sz;
	sq.col = col;

	return sq;
}

struct SPRITE
{
	///V2f pos;
	V3f pos;
	//V2f size;
	V3f size;
	V4f col;
	V2f tcoord_p;
	V2f tcoord_sz;
};

SPRITE StdSprite(V3f p = v3f(0.01f, 0.1f, 0.0f),
		 V3f sz = v3f(0.1f, 0.1f, 0.1f),
		 V4f col = COLOR_WHITE,
		 V2f tp = v2f(0.0f, 0.0f),
		 V2f tsz = v2f(0.1f, 0.1f))
{
	SPRITE s = {};
	s.pos = p;
	s.size = sz;
	s.col = col;
	s.tcoord_p = tp;
	s.tcoord_sz = tsz;

	return s;
}

struct MEM_POOL
{
	u8 *data;
	u32 max_size;
	u32 used;
	u32 count;
};

#define MAX_PARTICLES 1000
struct PARTICLE
{
	SPRITE *spr;
	V2f vel;
	bool dead;
	bool collided;
};

struct CURSOR
{
	V2f p;
	SOLCOL_QUAD *quad;
};

#include "cj_console.h"

#define TILEMAP_W 32
#define TILEMAP_H 34
struct GAME_HANDLER
{
	MEM_POOL line_batch;
	COLOR_LINE *cl1;

	MEM_POOL solcolQ_batch;
	SOLCOL_QUAD *sq1;

	MEM_POOL sprite_batch;
	V2f texture_map[5][TILEMAP_W * TILEMAP_H];

	u32 level_progress;

	CURSOR cursor;

	CJ_CONSOLE console = {};
    
	GAME_STATE state = game_state_main_menu;

	bool is_initialized = false;
};

////////////////// FUNC DECL ///////////////////////////////

void LoadTextureCoords(char *_filename, V2f *map, u32 w, u32 h);
void LoadTextureCoords(char *_filename, V2f *map, u32 w, u32 h)
{
		char *filename = _filename;
		FILE *f = fopen(filename, "r");
		if(f)
		{
			fseek(f, 0, SEEK_SET);
			const int size = 14 * w * h;
			char *buff = (char*)calloc(size, sizeof(char));
			fread(buff, sizeof(char), size, f);
			char *temp_buff = buff;
			V2f tp = {};
			u32 i = 0;
			while(*temp_buff != '\0')
			{
				char next_float[5] = {};
				next_float[0] = temp_buff[0];
				next_float[1] = temp_buff[1];
				next_float[2] = temp_buff[2];
				next_float[3] = temp_buff[3];
				next_float[4] = temp_buff[4];
				tp.x = atof(next_float);
				temp_buff += 6;


				next_float[0] = temp_buff[0];
				next_float[1] = temp_buff[1];
				next_float[2] = temp_buff[2];
				next_float[3] = temp_buff[3];
				next_float[4] = temp_buff[4];
				tp.y = atof(next_float);
				temp_buff += 6;
				if(*temp_buff == '\n')
					temp_buff++;

				map[i] = tp;
				i++;
			}
			fclose(f);
		}
}
void OnGameInit(CJ_PLATFORM *platform, CJ_RENDERER *renderer, GAME_HANDLER *game);
void OnGameUpdate(CJ_PLATFORM *platform, CJ_RENDERER *renderer, GAME_HANDLER *game);
void OnGameShutDown(GAME_HANDLER *game);

bool CursorInsideText(CJ_PLATFORM *platform, TEXT_INFO ti);
bool CursorInsideEntity(CJ_PLATFORM *platform, ENTITY *e);
bool CursorInside_SOLCOL_QUAD(CJ_PLATFORM *platform, SOLCOL_QUAD q);
bool CursorInside_SPRITE(CJ_PLATFORM *platform, SPRITE spr);

void UpdateVBO_COLOR_LINE(CJ_VBO *vbo, GAME_HANDLER *game);
void UpdateVBO_SOLCOL_QUAD(CJ_VBO *vbo, GAME_HANDLER *game);
void UpdateVBO_Entity(CJ_VBO *vbo, GAME_HANDLER *game);

void GetDirContent(TEXT_INFO *ti, char *storage, char *dir, u32 max_filesz_name);

TEXT_INFO DrawText(CJ_PLATFORM *platform, CJ_RENDERER *renderer, u32 font_texture_index, char *str, V2f pos, V4f col);
#define _DrawText(f, str, p, c) DrawText(platform, renderer, f, str, p ,c)

ENTITY *PushEntity(GAME_HANDLER *game, u32 size);
u8 *Push_COLOR_LINE(GAME_HANDLER *game, u32 size, u32 count);
u8 *Push_SOLCOL_QUAD(GAME_HANDLER *game, u32 size, u32 count);
u8 *Push_SPRITE(GAME_HANDLER *game, u32 size, u32 count);
#define Push(pool, type, count) (type*)Push_##type(pool, sizeof(type), count)


u8 *Push_COLOR_LINE(GAME_HANDLER *game, u32 size, u32 count)
{
	u32 total_size 	= size * count;
	u8 *result 	= 0;
	if(game->line_batch.used + total_size < game->line_batch.max_size)
	{
		result =  game->line_batch.data + game->line_batch.used;
		game->line_batch.used += total_size;
		game->line_batch.count += count;

	}
	else printf("Line batch overflow\n");

	return result;
}

u8 *Push_SOLCOL_QUAD(GAME_HANDLER *game, u32 size, u32 count)
{
	u32 total_size 	= size * count;
	u8 *result 	= 0;
	if(game->solcolQ_batch.used + total_size < game->solcolQ_batch.max_size)
	{
		result =  game->solcolQ_batch.data + game->solcolQ_batch.used;
		game->solcolQ_batch.used += total_size;
		game->solcolQ_batch.count += count;

	}
	else printf("solcolQ batch overflow\n");

	return result;
}

u8 *Push_SPRITE(GAME_HANDLER *game, u32 size, u32 count)
{
	u32 total_size 	= size * count;
	u8 *result 	= 0;
	if(game->sprite_batch.used + total_size < game->sprite_batch.max_size)
	{
		result =  game->sprite_batch.data + game->sprite_batch.used;
		game->sprite_batch.used += total_size;
		game->sprite_batch.count += count;

	}
	else printf("Sprite batch overflow\n");

	return result;
}


	
bool OnEntityClick(CJ_PLATFORM *platform, ENTITY *entity)
{
	bool result = false;

	if(CursorInsideEntity(platform, entity))
	{
		if(MousePressedOnce(GLFW_MOUSE_BUTTON_LEFT))
		{
			result = true;
		}
		else
		{
			result = false;
		}
	
	}

	return result;

}

////////////////////////////////////////////////////////////

void ftoa(float f, char *buffer, int rounding);
void ftoa(float f, char *buffer, int rounding)
{
	int left_part = (int)f;
	int temp_left_part = left_part;
	itoa(left_part, buffer, 10);

	int n_left_digits = 0;
	while(temp_left_part > 0)
	{
		n_left_digits++;
		temp_left_part /= 10;
	}

	buffer[n_left_digits] = '.';

	float right_part = f - (int)left_part;

	// point to 1 after the '.'
	char *p_buff = buffer + n_left_digits + 1;
	for(int i = 0; i < rounding; i++)
	{

		right_part *= 10;

		int next_digit = (int)right_part;
		itoa(next_digit, p_buff, 10);
		p_buff++;

		right_part -= (int)right_part;
	}
}


using namespace irrklang;
ISoundEngine *sound_engine;
ISound *sound_handler;
float sound_volume = 0.5f;
void OnGameInit(CJ_PLATFORM *platform, CJ_RENDERER *renderer, GAME_HANDLER *game)
{

	CJ_InitRandom();


	if(!game->is_initialized)
	{
		sound_engine = createIrrKlangDevice();
		sound_handler = sound_engine->play2D("assets\\sounds\\retrofunk.mp3", true, false, true);
		sound_handler->setVolume(sound_volume);

		u32 n_color_lines		= 100;
		game->line_batch.data 		= (u8*)calloc(n_color_lines, sizeof(COLOR_LINE));
		game->line_batch.max_size	= n_color_lines * sizeof(COLOR_LINE);

		u32 n_solidcolor_quads 		= 100;
		game->solcolQ_batch.data 	= (u8*)calloc(n_solidcolor_quads, sizeof(SOLCOL_QUAD));
		game->solcolQ_batch.max_size	= n_solidcolor_quads * sizeof(SOLCOL_QUAD);

		u32 n_sprites			= renderer->max_sprites_to_render;
		game->sprite_batch.data 	= (u8*)calloc(n_sprites, sizeof(SPRITE));
		game->sprite_batch.max_size	= n_sprites * sizeof(SPRITE);

		game->console.bkg_window	= Push(game, SOLCOL_QUAD, 1); 

		game->cursor.quad		= Push(game, SOLCOL_QUAD, 1);
		*game->cursor.quad		= StdSolColQuad(v2f(0.3f, 1.9f), v2f(0.02f, 0.003f), COLOR_GREEN);


		game->is_initialized = true;
	}
}

/////////////////////////////////////////////////////////////////// 


void OnGameUpdate(CJ_PLATFORM *platform, CJ_RENDERER *renderer, GAME_HANDLER *game)
{


	if(KeyPressed(platform, GLFW_KEY_DOWN))
	{
		sound_volume -= 0.01f;
		sound_handler->setVolume(sound_volume);
	}
	if(KeyPressed(platform, GLFW_KEY_UP))
	{
		sound_volume += 0.01f;
		sound_handler->setVolume(sound_volume);
	}

	static char input[64] = {};
	static float tick = 0.0f;
	static i32 n_chars_drawn = 0;
	tick += platform->time_per_frame;
	static bool green = true;
	if(tick >= 0.5f)
	{
		tick = 0.0f;
		if(green)
		{
			game->cursor.quad->col = COLOR_BLACK;
			green = false;
		}
		else
		{
			green = true;
			game->cursor.quad->col = COLOR_GREEN;
		}
	}

	static TEXT_INFO fixed_txt_i = {};
	static TEXT_INFO dyn_txt_i = {};


	if(*global_text_input != '\0')
	{
		char gc = *global_text_input;
		input[n_chars_drawn] = gc;
		n_chars_drawn++;
	}
	else if (KeyPressedOnce(GLFW_KEY_BACKSPACE))
	{
		char gc = '\0';
		n_chars_drawn--;

		if(n_chars_drawn <= 0)
			n_chars_drawn = 0;

		input[n_chars_drawn] = gc;
	}


	fixed_txt_i = _DrawText(3, "Enter name:", v2f(0.2f, 1.9f), COLOR_WHITE);  
	//game->cursor.quad->pos.x = 0.00f + (txt_i.sentence_size.x - txt_i.sentence_pos.x);
	game->cursor.quad->pos.x = (fixed_txt_i.sentence_size.x + fixed_txt_i.sentence_pos.x) + dyn_txt_i.sentence_size.x - dyn_txt_i.sentence_pos.x;
	dyn_txt_i = _DrawText(3, input, v2f(0.275f, 1.9f), COLOR_GREEN);  

	u32 console_buffer_size = 512;
	HandleConsole(&game->console, console_buffer_size);

	if(KeyPressedOnce(GLFW_KEY_ESCAPE))
		running = false;
    
}

void OnGameShutDown(GAME_HANDLER *game)
{
}

TEXT_INFO DrawText(CJ_PLATFORM *platform, CJ_RENDERER *renderer, u32 font_texture_index, char *str, V2f pos, V4f col)
{
	TEXT_INFO ti = {};
    
	char line[1024] = {};
	strcpy(line, str);
	
	// xp & yp is baseline in stbtt_GetPackedQuad();
	float yp = 0.0f;
	float xp = 0.0f;
	
	float t_pos_x = 0.0f;
	float t_pos_y = 0.0f;
	float t_sz_x = 0.0f;
	float t_sz_y = 0.0f;
	
	float c_x = 0.0f;
	float c_y = 0.0f;
	
	float c_w = 0.0f;
	float c_h = 0.0f;
	
	float biggest_h = 0.0f;
    
    
	TEXTUREINFO *tex_i 		= &renderer->texture_batch[font_texture_index];
	
	u32 n_chars 			= strlen(line);
	u32 size_vertices 		= n_chars * 6 * sizeof(CJ_VTX_SPRITE);
	CJ_VTX_SPRITE *vertices 	= (CJ_VTX_SPRITE*)calloc(n_chars * 6, sizeof(CJ_VTX_SPRITE));

	bool new_line = false;
	int n_new_lines = 0;
	float y_mod = -0.05f;
	float x_pos_sentece_start = 0.0f;
	bool calc_start_pos = false;
	float p = 0.0f;
	float diff = 0.0f;

	for(i32 i = 0; i < n_chars; i++)
	{
	    stbtt_GetPackedQuad(tex_i->packed_char, 
			    	tex_i->atlas_w, 
				tex_i->atlas_h, 
				line[i] - ' ', 
				&xp, &yp, 
				&tex_i->glyph_quad, 0);

	    
	    t_pos_x = tex_i->glyph_quad.s0;
	    t_pos_y = 1.0f - tex_i->glyph_quad.t1;
	    t_sz_x  = tex_i->glyph_quad.s1 - tex_i->glyph_quad.s0;
	    t_sz_y  = (1.0f - tex_i->glyph_quad.t0) - (1.0f - tex_i->glyph_quad.t1);
        


	    // Calculating position for new line
	    p = pos.x + (tex_i->glyph_quad.x0)  / 1080.0f; 
	    if(new_line)
	    {

		if(calc_start_pos == false)
		{
			diff = p - x_pos_sentece_start;
			calc_start_pos = true;
		}

		c_x = p - diff;
		new_line = false;

	    }
	    else
	    {
	    	c_x = pos.x + (tex_i->glyph_quad.x0)  / 1080.0f;
		c_x -= diff;
	    }

	    if(line[i] == '\n')
	    {
		    n_new_lines++;
		    new_line = true;
		    calc_start_pos = false;
		    //printf("new line\n");
	    }

	    if(i == 0)
		    x_pos_sentece_start = c_x;

	    c_y = (n_new_lines * y_mod) + pos.y + ((yp + (yp - tex_i->glyph_quad.y1))) / 960.0f;
	    
	    c_w = (tex_i->glyph_quad.x1 - tex_i->glyph_quad.x0)  / 1080.0f;
	    c_h = (tex_i->glyph_quad.y1 - tex_i->glyph_quad.y0)  / 960.0f;

        
	    //vertices[(i * 6) + 0].pos 	= v2f(c_x, c_y);
	    //vertices[(i * 6) + 1].pos 	= v2f(c_x, c_y + c_h);
	    //vertices[(i * 6) + 2].pos 	= v2f(c_x + c_w, c_y + c_h);
	   // vertices[(i * 6) + 3].pos 	= v2f(c_x, c_y);
	    //vertices[(i * 6) + 4].pos 	= v2f(c_x + c_w, c_y + c_h);
	    //vertices[(i * 6) + 5].pos 	= v2f(c_x + c_w, c_y);
	    vertices[(i * 6) + 0].pos 		= v3f(c_x, c_y, 0.0f);
	    vertices[(i * 6) + 1].pos 		= v3f(c_x, c_y + c_h, 0.0f);
	    vertices[(i * 6) + 2].pos 		= v3f(c_x + c_w, c_y + c_h, 0.0f);
	    vertices[(i * 6) + 3].pos 		= v3f(c_x, c_y, 0.0f);
	    vertices[(i * 6) + 4].pos 		= v3f(c_x + c_w, c_y + c_h, 0.0f);
	    vertices[(i * 6) + 5].pos 		= v3f(c_x + c_w, c_y, 0.0f);

	    vertices[(i * 6) + 0].col 	= col;
	    vertices[(i * 6) + 1].col 	= col;
	    vertices[(i * 6) + 2].col 	= col;
	    vertices[(i * 6) + 3].col 	= col;
	    vertices[(i * 6) + 4].col 	= col;
	    vertices[(i * 6) + 5].col 	= col;

	    vertices[(i * 6) + 0].tCoord = v2f(t_pos_x, t_pos_y);
	    vertices[(i * 6) + 1].tCoord = v2f(t_pos_x, t_pos_y + t_sz_y);
	    vertices[(i * 6) + 2].tCoord = v2f(t_pos_x + t_sz_x, t_pos_y + t_sz_y);
	    vertices[(i * 6) + 3].tCoord = v2f(t_pos_x, t_pos_y);
	    vertices[(i * 6) + 4].tCoord = v2f(t_pos_x + t_sz_x, t_pos_y + t_sz_y);
	    vertices[(i * 6) + 5].tCoord = v2f(t_pos_x + t_sz_x, t_pos_y);
        
	    if(i == 0)
		    ti.sentence_pos = v3f(c_x, c_y, 0.0f);
        
	    if(c_y + c_h > biggest_h)
		    biggest_h = c_y + c_h;

	    renderer->n_chars_drawn++;
	}
    

	
	u8 *vbo_text_tracker = (u8*)(renderer->vbo_text.base + renderer->vbo_text_index_tracker);
	if(renderer->vbo_text_index_tracker <= renderer->vbo_text.size)
	{
		memcpy(vbo_text_tracker, vertices, size_vertices);
		free(vertices);
		renderer->vbo_text_index_tracker += size_vertices;

		ti.sentence_size = v2f(c_x + c_w, biggest_h);
	}
	else
	{
		printf("Text buffer Overflow!\n");
		assert(renderer->vbo_text_index_tracker <= renderer->vbo_text.size);
	}
    
	return ti;
} 

bool CursorInsideText(CJ_PLATFORM *platform, TEXT_INFO ti)
{
	if(platform->cursor_pos.x > ti.sentence_pos.x &&
	   platform->cursor_pos.x < ti.sentence_size.x &&
	   platform->cursor_pos.y > ti.sentence_pos.y &&
	   platform->cursor_pos.y < ti.sentence_size.y)
		return true;
	else
		return false;
}

bool CursorInsideEntity(CJ_PLATFORM *platform, ENTITY *e)
{
	if(platform->cursor_pos.x > e->pos.x &&
	   platform->cursor_pos.x < e->pos.x + e->size.x &&
	   platform->cursor_pos.y > e->pos.y &&
	   platform->cursor_pos.y < e->pos.y + e->size.y)
		return true;
	else
		return false;
}

bool CursorInside_SOLCOL_QUAD(CJ_PLATFORM *platform, SOLCOL_QUAD q)
{
	if(platform->cursor_pos.x > q.pos.x &&
	   platform->cursor_pos.x < q.pos.x + q.size.x &&
	   platform->cursor_pos.y > q.pos.y &&
	   platform->cursor_pos.y < q.pos.y + q.size.y)
		return true;
	else
		return false;
}

bool CursorInside_SPRITE(CJ_PLATFORM *platform, SPRITE spr)
{
	if(platform->cursor_pos.x > spr.pos.x &&
	   platform->cursor_pos.x < spr.pos.x + spr.size.x &&
	   platform->cursor_pos.y > spr.pos.y &&
	   platform->cursor_pos.y < spr.pos.y + spr.size.y)
		return true;
	else
		return false;
}

void UpdateVBO_SOLCOL_QUAD(CJ_VBO *vbo, GAME_HANDLER *game)
{
	CJ_VTX_SOLCOL_QUAD *pVertex = (CJ_VTX_SOLCOL_QUAD*)vbo->base;

	SOLCOL_QUAD *sq = (SOLCOL_QUAD*)game->solcolQ_batch.data;
	for(int i = 0; i < game->solcolQ_batch.count; i++)
	{
		V2f p 	= sq->pos;
		V2f sz	= sq->size;
		V4f col	= sq->col;

		pVertex[(i * 6) + 0].pos = v2f(p.x	 , p.y);
		pVertex[(i * 6) + 1].pos = v2f(p.x	 , p.y + sz.y);
		pVertex[(i * 6) + 2].pos = v2f(p.x + sz.x, p.y + sz.y);
		pVertex[(i * 6) + 3].pos = v2f(p.x + sz.x, p.y + sz.y);
		pVertex[(i * 6) + 4].pos = v2f(p.x	 , p.y);
		pVertex[(i * 6) + 5].pos = v2f(p.x + sz.x, p.y);

		pVertex[(i * 6) + 0].col = col;
		pVertex[(i * 6) + 1].col = col;
		pVertex[(i * 6) + 2].col = col;
		pVertex[(i * 6) + 3].col = col;
		pVertex[(i * 6) + 4].col = col;
		pVertex[(i * 6) + 5].col = col;

		sq++;

	}

}

void UpdateVBO_COLOR_LINE(CJ_VBO *vbo, GAME_HANDLER *game)
{
	CJ_VTX_COL_LINE *pVertex = (CJ_VTX_COL_LINE*)vbo->base;

	COLOR_LINE *cl = (COLOR_LINE*)game->line_batch.data;
	// Updating the data that's sent to the VBO
	for(u32 i = 0; i < game->line_batch.count; i++)
	{
		V2f p1 		= cl->p1;
		V2f p2 		= cl->p2;
		V4f col 	= cl->col;
        
		pVertex[(i * 2) + 0].p 		= p1;
		pVertex[(i * 2) + 1].p 		= p2;

		pVertex[(i * 2) + 0].col 	= col;
		pVertex[(i * 2) + 1].col 	= col;
        
		cl++;
	}
}

void UpdateVBO_SPRITE(CJ_VBO *vbo, GAME_HANDLER *game)
{
	CJ_VTX_SPRITE *pVertex = (CJ_VTX_SPRITE*)vbo->base;
    
	SPRITE *spr = (SPRITE*)game->sprite_batch.data;
	// Updating the data that's sent to the VBO
	for(u32 i = 0; i < game->sprite_batch.count; i++)
	{
		//V2f pos 		= spr[i].pos;
		V3f pos 		= spr[i].pos;
		V3f size 		= spr[i].size;
		V4f col 		= spr[i].col;
		V2f tCoord_p		= spr[i].tcoord_p;
		V2f tCoord_sz		= spr[i].tcoord_sz;

		//pVertex[(i * 4) + 0].pos = v2f(pos.x, 		pos.y);
		//pVertex[(i * 4) + 1].pos = v2f(pos.x, 		pos.y + size.y);
		//pVertex[(i * 4) + 2].pos = v2f(pos.x + size.x,  pos.y + size.y);
		//pVertex[(i * 4) + 3].pos = v2f(pos.x + size.x,  pos.y);
		pVertex[(i * 4) + 0].pos = v3f(pos.x, 		pos.y, pos.z);
		pVertex[(i * 4) + 1].pos = v3f(pos.x, 		pos.y + size.y, pos.z);
		pVertex[(i * 4) + 2].pos = v3f(pos.x + size.x,  pos.y + size.y, pos.z);
		pVertex[(i * 4) + 3].pos = v3f(pos.x + size.x,  pos.y, pos.z);
        
		pVertex[(i * 4) + 0].col = col;
		pVertex[(i * 4) + 1].col = col;
		pVertex[(i * 4) + 2].col = col;
		pVertex[(i * 4) + 3].col = col;
        
		pVertex[(i * 4) + 0].tCoord = v2f(tCoord_p.x, 		    tCoord_p.y);
		pVertex[(i * 4) + 1].tCoord = v2f(tCoord_p.x, 		    tCoord_p.y + tCoord_sz.y);
		pVertex[(i * 4) + 2].tCoord = v2f(tCoord_p.x + tCoord_sz.x, tCoord_p.y + tCoord_sz.y);
		pVertex[(i * 4) + 3].tCoord = v2f(tCoord_p.x + tCoord_sz.y, tCoord_p.y);
        
	}
}

void GetDirContent(TEXT_INFO *ti, char *storage, char *dir, u32 max_filesz_name)
{
	ti->n_files_in_dir = 0;
	WIN32_FIND_DATA find_data 		= {};
	LARGE_INTEGER file_sz			= {};
	HANDLE hFind				= {};
    
	strcpy(storage, dir);
	strcat(storage, "/*");
	ti->n_files_in_dir++;
    
	hFind = FindFirstFile(storage, &find_data);
    
	if(hFind == INVALID_HANDLE_VALUE) 
		printf("couldn't find file in %s\n", storage);
	else
		storage += max_filesz_name;
    
	while(FindNextFile(hFind, &find_data) != 0)
	{
		if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			strcpy(storage, find_data.cFileName);
			storage += max_filesz_name;
			ti->n_files_in_dir++;
            
		}
		else
		{
			char *dir_assets = "assets/";
			strcpy(storage, dir_assets);
			strcat(storage, find_data.cFileName);
			storage += max_filesz_name;
			ti->n_files_in_dir++;
		}
	}
    
}
#endif
