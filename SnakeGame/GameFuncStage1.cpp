#include "GameFunc.h"
#include "GameFuncStage1.h"

Stage1::Stage1()
{
	//윈도우 정중앙 배치를 위한 변수 선언

	SDL_GetWindowSize(g_window, &win_w, &win_h);

	//BG
	{
		SDL_Surface* temp_surface = IMG_Load("../../Resources/BG.png");
		bg_texture = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
		SDL_FreeSurface(temp_surface);

		SDL_QueryTexture(bg_texture, NULL, NULL, &bg_source_rect.w, &bg_source_rect.h);
		bg_source_rect.x = 0;
		bg_source_rect.y = 0;
		bg_destination_rect.x = 0;
		bg_destination_rect.y = 0;
		bg_destination_rect.w = 1000;
		bg_destination_rect.h = 700;
	}

	//Game Over BG
	{
		SDL_Surface* temp_surface = IMG_Load("../../Resources/Over.png");
		over_texture = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
		SDL_FreeSurface(temp_surface);

		SDL_QueryTexture(over_texture, NULL, NULL, &over_source_rect.w, &over_source_rect.h);
		over_source_rect.x = 0;
		over_source_rect.y = 0;
		over_destination_rect.x = 0;
		over_destination_rect.y = 0;
		over_destination_rect.w = 1000;
		over_destination_rect.h = 700;
	}

	//Snake Head
	{
		SDL_Surface* g_surface = IMG_Load("../../Resources/sprites.png");
		sprites_texture = SDL_CreateTextureFromSurface(g_renderer, g_surface);
		SDL_FreeSurface(g_surface);

		sn_source_rect.x = 13;
		sn_source_rect.y = 145;
		sn_source_rect.w = 106;
		sn_source_rect.h = 106;

		sn_destination_rect.x = 500;
		sn_destination_rect.y = 350;
		sn_destination_rect.w = 50;
		sn_destination_rect.h = 50;
	}

	//Snake Tail
	{
		SDL_Surface* g_surface = IMG_Load("../../Resources/sprites.png");
		sprites_texture = SDL_CreateTextureFromSurface(g_renderer, g_surface);
		SDL_FreeSurface(g_surface);

		tail_srect.x = 12;
		tail_srect.y = 262;
		tail_srect.w = 109;
		tail_srect.h = 109;

		tail_drect.w = 50;
		tail_drect.h = 50;
	}

	//Item
	{
		SDL_Surface* g_surface = IMG_Load("../../Resources/sprites.png");
		sprites_texture = SDL_CreateTextureFromSurface(g_renderer, g_surface);
		SDL_FreeSurface(g_surface);

		eg_source_rect.x = 19;
		eg_source_rect.y = 20;
		eg_source_rect.w = 80;
		eg_source_rect.h = 87;

		eg_destination_rect.x = 0;
		eg_destination_rect.y = 0;
		eg_destination_rect.w = 50;
		eg_destination_rect.h = 50;
	}




	//Game Over text
	{

		game_font = TTF_OpenFont("../../Resources/DungGeunMo.ttf", 100);
		SDL_Color red = { 220, 53, 53, 0 };
		SDL_Surface* tmp_surface = TTF_RenderText_Blended(game_font, "Game Over...", red);

		game_over_rect.x = 0;
		game_over_rect.y = 0;
		game_over_rect.w = tmp_surface->w;
		game_over_rect.h = tmp_surface->h;

		game_over_text = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);

		SDL_FreeSurface(tmp_surface);
	}


	game_font = TTF_OpenFont("../../Resources/DungGeunMo.ttf", 50);
	//score text
	{
		SDL_Color black = { 255, 255, 255, 0 };
		SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(game_font, CW2A(L"점수: ", CP_UTF8), black);

		score_text_rect.x = 0;
		score_text_rect.y = 0;
		score_text_rect.w = tmp_surface->w;
		score_text_rect.h = tmp_surface->h;

		score_text_text = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);

		SDL_FreeSurface(tmp_surface);
	}

	//Score num
	{
		SDL_Color white = { 255, 255, 255, 0 };
		SDL_Surface* tmp_surface = TTF_RenderText_Blended(game_font, "00", white);

		score_num_rect.x = 50;
		score_num_rect.y = 0;
		score_num_rect.w = tmp_surface->w;
		score_num_rect.h = tmp_surface->h;

		score_num_text = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);

		SDL_FreeSurface(tmp_surface);
	}



	//Var Init
	{
		dir_state = 0;
		count = 0;
		speed = 120;
		score = 0;
		isGet = false;
		gameOver = false;
		posX = sn_destination_rect.x;
		posY = sn_destination_rect.y;
	}

}


Stage1::~Stage1()
{
	SDL_DestroyTexture(sprites_texture);
	SDL_DestroyTexture(bg_texture);
	SDL_DestroyTexture(over_texture);
	//if (sound_effect) Mix_FreeChunk(sound_effect);
	//if (bg_music) Mix_FreeMusic(bg_music);
	TTF_CloseFont(game_font);
}

void Stage1::UpdateScoreTexture(int ms)
{
	if (score_num_text != 0)
	{
		SDL_DestroyTexture(score_num_text);
		score_num_text = 0;
	}

	std::string str;

	if (ms < 10)
		str = "0" + std::to_string((long long)ms);
	else
		str = std::to_string((long long)ms);


	SDL_Color yellow = { 255, 255, 255, 0 };
	if (score > 7)
		yellow = { 255,255,0,0 };
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(game_font, str.c_str(), yellow);

	score_num_rect.x = 0;
	score_num_rect.y = 0;
	score_num_rect.w = tmp_surface->w;
	score_num_rect.h = tmp_surface->h;

	score_num_text = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);

	SDL_FreeSurface(tmp_surface);
}

void Stage1::Food()
{
	int a, b;
	while (true)
	{
		a = (rand() % 19) * SIZE;
		b = (rand() % 13) * SIZE;

		for (auto iter = L.begin(); iter != L.end(); iter++)
		{
			if (a == iter->x && b == iter->y)
				continue;
			else
			{
				eg_destination_rect.x = a;
				eg_destination_rect.y = b;
				return;
			}
		}
	}
}

void Stage1::Eat()
{
	if (sn_destination_rect.x >= eg_destination_rect.x &&
		sn_destination_rect.y >= eg_destination_rect.y &&
		sn_destination_rect.x < eg_destination_rect.x + eg_destination_rect.w &&
		sn_destination_rect.y < eg_destination_rect.y + eg_destination_rect.h)
	{
		isGet = false;
		count++;

		if (speed > 5)
			speed -= 5;

		if (score > 7)
		{
			eg_source_rect.x = 129;
			eg_source_rect.y = 21;
			eg_source_rect.w = 78;
			eg_source_rect.h = 85;
			score += 5;
		}
		else
			score += 1;
	}
}


void Stage1::AddTail()
{
	auto iter = L.begin();

	L.push_front({ posX,posY });

	Eat();

	if (isGet == true)
		L.pop_back();

}

void Stage1::Move()
{
	if (dir_state == LEFT)
	{
		posX -= SIZE;
	}
	else if (dir_state == RIGHT)
	{
		posX += SIZE;
	}
	else if (dir_state == UP)
	{
		posY -= SIZE;
	}
	else if (dir_state == DOWN)
	{
		posY += SIZE;
	}

	if (posX > 975 || posX < 0 || posY>675 || posY < 0)
		gameOver = true;

	auto iter = L.begin();
	int x,y;
	x = iter->x;
	y = iter->y;
	iter++;

	for (; iter != L.end(); iter++)
		if (posX == iter->x && posY == iter->y)
			gameOver=true;
}

void Stage1::Update()
{
	UpdateScoreTexture(score);
	Move();
	AddTail();
	SDL_Delay(speed);
}


void Stage1::Render()
{
	SDL_RenderCopy(g_renderer, bg_texture, &bg_source_rect, &bg_destination_rect);

	if (isGet == false)
	{
		Food();
		isGet = true;
	}

	SDL_RenderCopy(g_renderer, sprites_texture, &eg_source_rect, &eg_destination_rect);

	auto iter = L.begin();
	sn_destination_rect.x = iter->x;
	sn_destination_rect.y = iter->y;
	SDL_RenderCopy(g_renderer, sprites_texture, &sn_source_rect, &sn_destination_rect);

	if (count != 0)
	{
		iter++;
		for (; iter != L.end();iter++)
		{
			tail_drect.x = iter->x;
			tail_drect.y = iter->y;
			SDL_RenderCopy(g_renderer, sprites_texture, &tail_srect, &tail_drect);
		}
	}

	//Score Text
	{
		SDL_Rect r;
		r.x = score_text_rect.x;
		r.y = score_text_rect.y;
		r.w = score_text_rect.w;
		r.h = score_text_rect.h;
		SDL_RenderCopy(g_renderer, score_text_text, 0, &r);
	}

	//Score Text
	{
		SDL_Rect r;
		r.x = 130;
		r.y = score_num_rect.y;
		r.w = score_num_rect.w;
		r.h = score_num_rect.h;
		SDL_RenderCopy(g_renderer, score_num_text, 0, &r);
	}



	if (gameOver == true)
	{

		SDL_SetTextureAlphaMod(over_texture, 160);
		SDL_SetTextureBlendMode(over_texture, SDL_BLENDMODE_BLEND);
		SDL_RenderCopy(g_renderer, over_texture, &over_source_rect, &over_destination_rect);

		SDL_Rect r;
		r.x = win_w / 2 - game_over_rect.w / 2;
		r.y = win_h / 2 - game_over_rect.h / 2;
		r.w = game_over_rect.w;
		r.h = game_over_rect.h;
		SDL_RenderCopy(g_renderer, game_over_text, 0, &r);
	}



	SDL_RenderPresent(g_renderer); // draw to the screen
}


void Stage1::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:

			if (event.key.keysym.sym == SDLK_ESCAPE)
				g_flag_running = false;


			if (event.key.keysym.sym == SDLK_LEFT&&gameOver==false)
			{
				if (dir_state != RIGHT || count == 0)
				{
					sn_source_rect.x = 136;
					sn_source_rect.y = 145;
					dir_state = LEFT;
				}
			}

			if (event.key.keysym.sym == SDLK_RIGHT && gameOver == false)
			{
				if (dir_state != LEFT || count == 0)
				{
					sn_source_rect.x = 382;
					sn_source_rect.y = 145;
					dir_state = RIGHT;
				}
			}

			if (event.key.keysym.sym == SDLK_UP && gameOver == false)
			{
				if (dir_state != DOWN || count == 0)
				{
					sn_source_rect.x = 13;
					sn_source_rect.y = 145;
					dir_state = UP;
				}
			}

			if (event.key.keysym.sym == SDLK_DOWN && gameOver == false)
			{
				if (dir_state != UP || count == 0)
				{
					sn_source_rect.x = 259;
					sn_source_rect.y = 145;
					dir_state = DOWN;
				}
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (gameOver==true&&(event.button.button == SDL_BUTTON_LEFT|| event.button.button == SDL_BUTTON_RIGHT))
				g_current_game_phase = PHASE_ENDING;

		default:
			break;
		}
	}
}



