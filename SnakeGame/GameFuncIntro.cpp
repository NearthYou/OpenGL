#include "GameFunc.h"
#include "GameFuncIntro.h"



Intro::Intro()
{
	// For Texture
	
	SDL_Surface* temp_surface = IMG_Load("../../Resources/intro.png");
	texture_intro = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	SDL_QueryTexture(texture_intro, NULL, NULL, &source_rectangle_intro.w, &source_rectangle_intro.h);
	destination_rectangle_intro.x = source_rectangle_intro.x = 0;
	destination_rectangle_intro.y = source_rectangle_intro.y = 0;
	destination_rectangle_intro.w = 1000;
	destination_rectangle_intro.h = 700;

	start_destination_rectange.x=302;
	start_destination_rectange.y=580;
	start_destination_rectange.w=391;
	start_destination_rectange.h=73;
}

Intro::~Intro()
{
	SDL_DestroyTexture(texture_intro); //±³¼ö´ÔÀÌ »©¸Ô¾ú´Ù°í ÇÑ°Ô ÀÌ°Å¾ß?
}

void Intro::Update()
{
}


void Intro::Render()
{
	SDL_RenderCopy(g_renderer, texture_intro, &source_rectangle_intro, &destination_rectangle_intro);
	SDL_RenderPresent(g_renderer); // draw to the screen
}



void Intro::HandleEvents()
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
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				int mouse_x = event.button.x;
				int mouse_y = event.button.y;

				if (mouse_x > start_destination_rectange.x &&
					mouse_y > start_destination_rectange.y &&
					mouse_x < start_destination_rectange.x + start_destination_rectange.w &&
					mouse_y < start_destination_rectange.y + start_destination_rectange.h
					)
				{
					g_current_game_phase = PHASE_STAGE1;
				}
			}
			break;

		default:
			break;
		}
	}
}


