#pragma once

#include "SDL.h"

class Ending : public PhaseInterface
{
public:
	Ending();
	~Ending();
	virtual void HandleEvents();
	virtual void Update();
	virtual void Render();

private:
	SDL_Texture* texture_ending; // the SDL_Texture 
	SDL_Rect source_rectangle_ending; // the rectangle for source image
	SDL_Rect destination_rectangle_ending; // for destination

	SDL_Rect restart_destination_rectangle;
};