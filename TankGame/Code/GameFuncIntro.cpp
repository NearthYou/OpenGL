#include "GameFunc.h"
#include "GameFuncIntro.h"
#include "GameFuncStage1.h"

static SDL_Texture* g_texture_intro; // the SDL_Texture 
static SDL_Rect g_source_rectangle_intro; // the rectangle for source image
static SDL_Rect g_destination_rectangle_intro; // for destination

Mix_Music* intro_music;

void Init_Intro()
{
	// For Texture
	SDL_Surface* temp_surface = IMG_Load("../../Resources/intro.png");
	g_texture_intro = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	SDL_QueryTexture(g_texture_intro, NULL, NULL, &g_source_rectangle_intro.w, &g_source_rectangle_intro.h);
	g_destination_rectangle_intro.x = g_source_rectangle_intro.x = 0;
	g_destination_rectangle_intro.y = g_source_rectangle_intro.y = 0;
	g_destination_rectangle_intro.w = 1000;
	g_destination_rectangle_intro.h = 700;

	//인트로 음악
	Mix_VolumeMusic(20);
	intro_music = Mix_LoadMUS("../../Resources/intro_music.mp3");
	if (!intro_music)
	{
		printf(" %s\n", Mix_GetError());
	}
	Mix_PlayMusic(intro_music, 0);
}

void Update_Intro()
{
}

void Render_Intro()
{
	SDL_RenderCopy(g_renderer, g_texture_intro, &g_source_rectangle_intro, &g_destination_rectangle_intro);
	SDL_RenderPresent(g_renderer); // draw to the screen
}

void HandleEvents_Intro()
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
				//마우스 클릭시 음악을 멈추고 볼륨을 줄인 뒤 Init_Stage1 재호출 후 페이즈 넘김
				Mix_PauseMusic();
				Mix_VolumeMusic(0);
				Init_Stage1();
				g_current_game_phase = PHASE_STAGE1;
			}
			break;

		default:
			break;
		}
	}
}

void Clear_Intro()
{
	SDL_DestroyTexture(g_texture_intro);
	if (intro_music) Mix_FreeMusic(intro_music);
}