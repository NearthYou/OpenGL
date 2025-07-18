
#include "GameFunc.h"
#include "GameFuncEnding.h"

static SDL_Texture* g_texture_ending; // the SDL_Texture 
static SDL_Rect g_source_rectangle_ending; // the rectangle for source image
static SDL_Rect g_destination_rectangle_ending; // for destination

Mix_Music* intro_music2;

void Init_Ending()
{
	// For Texture
	
	SDL_Surface* temp_surface = IMG_Load("../../Resources/ending.png");
	g_texture_ending = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	SDL_QueryTexture(g_texture_ending, NULL, NULL, &g_source_rectangle_ending.w, &g_source_rectangle_ending.h);
	g_destination_rectangle_ending.x = 0;
	g_destination_rectangle_ending.y = 0;
	g_source_rectangle_ending.x = 0;
	g_source_rectangle_ending.y = 0;
	g_destination_rectangle_ending.w = 1000;
	g_destination_rectangle_ending.h = 700;

	//¿Œ∆Æ∑Œ ¿Ωæ«
	intro_music2 = Mix_LoadMUS("../../Resources/intro_music.mp3");
	if (!intro_music2)
	{
		printf(" %s\n", Mix_GetError());
	}
}

void Update_Ending()
{
}

void Render_Ending()
{
	SDL_RenderCopy(g_renderer, g_texture_ending, &g_source_rectangle_ending, &g_destination_rectangle_ending);

	SDL_RenderPresent(g_renderer); // draw to the screen
}

void HandleEvents_Ending()
{
	SDL_Event event;
	if(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				g_flag_running = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
		
			// If the mouse left button is pressed. 
			if ( event.button.button == SDL_BUTTON_RIGHT )
			{
				Mix_PauseMusic();
				Mix_VolumeMusic(0);
				g_current_game_phase = PHASE_INTRO;
				Mix_VolumeMusic(20);
				Mix_PlayMusic(intro_music2, 0);

			}
			break;

		default:
			break;
		}
	}
}

void Clear_Ending()
{
	SDL_DestroyTexture(g_texture_ending);
	if (intro_music2) Mix_FreeMusic(intro_music2);
}