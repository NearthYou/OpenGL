#pragma once

#include "SDL.h"
#include "Room.h"
#include "Ball.h"

#define MAX_BALL_NUM 1000

class ProjectileGame
{
public:
	ProjectileGame();
	~ProjectileGame();
	void Render();	
	void Update();
	void HandleEvents();

protected:
	void AddNewBall();

protected:

	// Room
	Room room_;


	// Balls
	int num_of_balls_;
	Ball *balls_[MAX_BALL_NUM];

	// Ball Texture
	SDL_Texture* ball_texture_; 
	SDL_Rect ball_src_rectangle_;


	// Mouse
	int current_mouse_win_x_;
	int current_mouse_win_y_;
	int before_mouse_win_x_;
	int before_mouse_win_y_;
	bool shoot;

	float timer;
	float count;
	int num;


	struct Guide {
		double x;
		double y;
	}; Guide route[10];

};