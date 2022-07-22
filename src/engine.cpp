// TODO(calle): fixa så att DrawText() kan printa olika storlekar på fonts..... kolla i cj_renderer.h
// 				- Draw() i UpdateRenderer() måste veta vilken texture_batch / tex_i some ska användas 

bool running = true;

#include "cj_engine.h"
#include "game.h"

GLenum err = 0;

// Sound

int main()
{
    
	CJ_PLATFORM platform 	= CreatePlatform(720, 620, "Platform", CJ_WINDOWED);
	//CJ_PLATFORM platform 	= CreatePlatform(1920, 1080, "Platform", CJ_FULLSCREEN);
	CJ_RENDERER renderer 	= CreateRenderer();
	CJ_InitRandom();
	GAME_HANDLER game 	= {};

    
	while(!glfwWindowShouldClose(platform.window) && running)
	{
        
		//////// PLATFORM ///////////
		UpdatePlatform(&platform);

		//////// GAME ///////////
		OnGameInit(&platform, &renderer, &game);
		OnGameUpdate(&platform, &renderer, &game);

		UpdateVBO_COLOR_LINE(&renderer.vbo_lines, &game);
		UpdateVBO_SOLCOL_QUAD(&renderer.vbo_solcol_quad, &game);
		UpdateVBO_SPRITE(&renderer.vbo_sprite, &game);
        
		//////// RENDERER ///////////
		UpdateRenderer(&renderer, &platform);
        
        
		SwapBuffers(platform);
	}

	EndConsole(&game.console);
	OnGameShutDown(&game);

    
    
    
	glfwTerminate();
    
    
    return 0;
}


