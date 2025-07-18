#pragma once

#include "SDL.h"
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4
#define SIZE 50


class Stage1 : public PhaseInterface
{
public:
	Stage1();
	~Stage1();

	virtual void HandleEvents();
	virtual void Update();
	virtual void Render();
	virtual void Food();
	virtual void AddTail();
	virtual void Eat();
	virtual void Move();
	virtual void UpdateScoreTexture(int ms);

private:
	//BG
	SDL_Texture* bg_texture;
	SDL_Rect bg_source_rect;
	SDL_Rect bg_destination_rect;


	//Game Over BG
	SDL_Texture* over_texture;
	SDL_Rect over_source_rect;
	SDL_Rect over_destination_rect;


	//Snake
	SDL_Texture* sprites_texture;
	SDL_Rect sn_source_rect;
	SDL_Rect sn_destination_rect;

	//Tail
	SDL_Rect tail_srect;
	SDL_Rect tail_drect;


	//Egg
	SDL_Rect eg_source_rect;
	SDL_Rect eg_destination_rect;

	//TTF
	TTF_Font* game_font;

	//Text
	SDL_Texture* game_over_text;
	SDL_Rect game_over_rect;
	SDL_Texture* score_text_text;
	SDL_Rect score_text_rect;
	SDL_Texture* score_num_text;
	SDL_Rect score_num_rect;

	class Node
	{
	public:
		int x;
		int y;


		Node()
		{
			x = 0;
			y = 0;
		}

		Node(int x_, int y_)
		{
			x = x_;
			y = y_;
		}
	};
	std::list<Node> L;
	int dir_state;
	int count;
	bool isGet;
	int speed;
	int posX;
	int posY;
	int score;
	bool gameOver;
	int win_w, win_h;
};
