#pragma once

#include "SDL.h"
#include "GameFuncStage1.h"

class Intro : public PhaseInterface
{
public:
	Intro();
	~Intro();
	virtual void HandleEvents();
	virtual void Update();
	virtual void Render();

private:
	SDL_Texture* texture_intro; // the SDL_Texture 
	SDL_Rect source_rectangle_intro; // the rectangle for source image
	SDL_Rect destination_rectangle_intro; // for destination

	SDL_Rect start_destination_rectange;
};
