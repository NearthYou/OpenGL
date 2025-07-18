
#include "GameFunc.h"
#include "GameFuncEnding.h"



Ending::Ending()
{
	// For Texture
	
	SDL_Surface* temp_surface = IMG_Load("../../Resources/ending.png");
	texture_ending = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	SDL_QueryTexture(texture_ending, NULL, NULL, &source_rectangle_ending.w, &source_rectangle_ending.h);
	destination_rectangle_ending.x = 0;
	destination_rectangle_ending.y = 0;
	source_rectangle_ending.x = 0;
	source_rectangle_ending.y = 0;
	destination_rectangle_ending.w = source_rectangle_ending.w;
	destination_rectangle_ending.h = source_rectangle_ending.h;

	restart_destination_rectangle.x = 354;
	restart_destination_rectangle.y = 604;
	restart_destination_rectangle.w = 286;
	restart_destination_rectangle.h = 50;
}

Ending::~Ending()
{
	SDL_DestroyTexture(texture_ending);
}

void Ending::Update()
{
}


void Ending::Render()
{
	SDL_RenderCopy(g_renderer, texture_ending, &source_rectangle_ending, &destination_rectangle_ending);
	SDL_RenderPresent(g_renderer); // draw to the screen
}



void Ending::HandleEvents()
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
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// Get the cursor's x position.
				int mouse_x = event.button.x;
				int mouse_y = event.button.y;

				if (mouse_x > restart_destination_rectangle.x &&
					mouse_y > restart_destination_rectangle.y &&
					mouse_x < restart_destination_rectangle.x + restart_destination_rectangle.w &&
					mouse_y < restart_destination_rectangle.y + restart_destination_rectangle.h
					)
				{
					g_current_game_phase = PHASE_INTRO;
					reinit = true;
				}
			}
			break;

		default:
			break;
		}
	}
}


