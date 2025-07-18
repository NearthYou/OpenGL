#include "GameFunc.h"
#include "GameFuncStage1.h"

#pragma region 변수 선언

//배경
static SDL_Texture* g_texture_stage1; 
static SDL_Rect g_source_rectangle_stage1; 
static SDL_Rect g_destination_rectangle_stage1;

//사람
static SDL_Texture* sheet_texture;
static SDL_Rect man_source_rect;
static SDL_Rect man_destination_rect;

//탱크
static SDL_Rect tank_source_rect;
static SDL_Rect tank_destination_rect;

//상자
static SDL_Rect chest_source_rect;
static SDL_Rect chest_destination_rect;

//대포알
static SDL_Rect cannon_source_rect;
static SDL_Rect cannon_destination_rect;

//텍스트
static TTF_Font* game_font;
static SDL_Texture* bullet_text;
static SDL_Rect bullet_text_rect;
static SDL_Texture* ride_text;
static SDL_Rect ride_text_rect;

//음악
static Mix_Music* ending_music;
static Mix_Chunk* ride;
static Mix_Chunk* fire;
static Mix_Chunk* item;

//기타
static int count;
static int tank_state;
static bool isRide;
static bool isGet;

struct Rocket {
	int x;
	int y;
	int isFire;
	int direction;
}; Rocket rocket[6];

#pragma endregion

void Init_Stage1()
{	
	//윈도우 정중앙 배치를 위한 변수 선언
	int win_w, win_h;
	SDL_GetWindowSize(g_window, &win_w, &win_h);

	//BG
	{
		SDL_Surface* temp_surface = IMG_Load("../../Resources/BG.png");
		g_texture_stage1 = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
		SDL_FreeSurface(temp_surface);

		SDL_QueryTexture(g_texture_stage1, NULL, NULL, &g_source_rectangle_stage1.w, &g_source_rectangle_stage1.h);
		g_source_rectangle_stage1.x = 0;
		g_source_rectangle_stage1.y = 0;
		g_destination_rectangle_stage1.x = 0;
		g_destination_rectangle_stage1.y = 0;
		g_destination_rectangle_stage1.w = 1000;
		g_destination_rectangle_stage1.h = 700;
	}

	//Man
	{
		SDL_Surface* g_surface = IMG_Load("../../Resources/Sprites.png");
		sheet_texture = SDL_CreateTextureFromSurface(g_renderer, g_surface);
		SDL_FreeSurface(g_surface);

		man_source_rect.x = 0;
		man_source_rect.y = 0;
		man_source_rect.w = 66;
		man_source_rect.h = 60;

		man_destination_rect.x = 200;
		man_destination_rect.y = 200;
		man_destination_rect.w = man_source_rect.w/2+10;
		man_destination_rect.h = man_source_rect.h-10;
	}

	//tank
	{		
		tank_source_rect.x = 386;
		tank_source_rect.y = 17;
		tank_source_rect.w = 64;
		tank_source_rect.h = 77;

		tank_destination_rect.x = win_w / 2 - tank_source_rect.w / 2;
		tank_destination_rect.y = win_h / 2 - tank_source_rect.h / 2;
		tank_destination_rect.w = tank_source_rect.w;
		tank_destination_rect.h = tank_source_rect.h;
	}

	//Chest
	{
		chest_source_rect.x = 300;
		chest_source_rect.y = 5;
		chest_source_rect.w = 63;
		chest_source_rect.h = 58;

		chest_destination_rect.x = 50;
		chest_destination_rect.y = 50;
		chest_destination_rect.w = chest_source_rect.w;
		chest_destination_rect.h = chest_source_rect.h;
	}

	//Cannon Ball
	{
		cannon_source_rect.x = 324;
		cannon_source_rect.y = 95;
		cannon_source_rect.w = 16;
		cannon_source_rect.h = 18;

		cannon_destination_rect.x = tank_destination_rect.x + 24;
		cannon_destination_rect.y = tank_destination_rect.y - 20;
		cannon_destination_rect.w = cannon_source_rect.w;
		cannon_destination_rect.h = cannon_source_rect.h;
	}

	//Music
	{
		ending_music = Mix_LoadMUS("../../Resources/ending_music.mp3");
		if (!ending_music)
		{
			printf(" %s\n", Mix_GetError());
		}
	}

	//Sound Effect
	{
		ride = Mix_LoadWAV("../../Resources/ride.mp3");
		if (ride == NULL)
		{
			printf("Couldn't load the wav: %s\n", Mix_GetError());
		}

		fire = Mix_LoadWAV("../../Resources/shoot.mp3");
		if (fire == NULL)
		{
			printf("Couldn't load the wav: %s\n", Mix_GetError());
		}

		item = Mix_LoadWAV("../../Resources/item.mp3");
		if (item == NULL)
		{
			printf("Couldn't load the wav: %s\n", Mix_GetError());
		}
	}

	//Text
	{
		game_font = TTF_OpenFont("../../Resources/DungGeunMo.ttf", 50);
		SDL_Color black = { 0, 0, 0, 0 };
		SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(game_font, CW2A(L"탑승중 ", CP_UTF8), black);

		ride_text_rect.x = 0;
		ride_text_rect.y = 0;
		ride_text_rect.w = tmp_surface->w;
		ride_text_rect.h = tmp_surface->h;

		ride_text = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);

		SDL_FreeSurface(tmp_surface);
	}

	//변수 초기화
	{
		isRide = false;
		isGet = false;
		tank_state = 0;
		count = 0;
		
		for (int i = 0; i < 5; i++)
		{
			rocket[i].x = cannon_destination_rect.x;
			rocket[i].y = cannon_destination_rect.y;
			rocket[i].isFire = 0;
			rocket[i].direction = 2;
		}

		game_font = TTF_OpenFont("../../Resources/DungGeunMo.ttf", 100);
	}

}

void UpdateCountTexture(int ms)
{
	if (bullet_text != 0)
	{
		SDL_DestroyTexture(bullet_text);
		bullet_text = 0;
	}

	SDL_Color black = { 0, 0, 0, 0 };
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(game_font, std::to_string((long long)ms).c_str(), black);

	bullet_text_rect.x = 0;
	bullet_text_rect.y = 0;
	bullet_text_rect.w = tmp_surface->w;
	bullet_text_rect.h = tmp_surface->h;

	bullet_text = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);

	SDL_FreeSurface(tmp_surface);
}

void Fire(int num)
{
	//대포알의 위치를 입력 받아 그리는 함수
	SDL_Rect c = cannon_destination_rect;
	c.x = rocket[num].x;
	c.y = rocket[num].y;

	SDL_RenderCopy(g_renderer, sheet_texture, &cannon_source_rect, &c);
}

void CannonScreenOut(int num)
{
	//대포알 전진 함수 
	switch (rocket[num].direction)
	{
	case 0:
		rocket[num].x -= 25;
		break;
	case 1:
		rocket[num].x += 25;
		break;
	case 2:
		rocket[num].y -= 25;
		break;
	case 3:
		rocket[num].y += 25;
		break;
	break;
	}

	//화면 끝에 걸리면 발사 종료
	if (rocket[num].x < -60 || rocket[num].x >= 1000 ||
		rocket[num].y < -60 || rocket[num].y >= 700)
	{
		rocket[num].isFire = 0;
	}
}

void Update_Stage1()
{
	//대포알의 개수를 나타내는 count변수를 텍스트로 변환
	UpdateCountTexture(count); 

	//대포알 전진
	for (int i = count; i < 6; i++)
		if (rocket[i].isFire == 1) CannonScreenOut(i);

	//캐릭터 Screen Out시 위치 재설정
	{
		if (man_destination_rect.x >= 1000)
		{
			man_destination_rect.x = -60;
		}
		else if (man_destination_rect.x < -60)
		{
			man_destination_rect.x = 1000;
		}
		else if (man_destination_rect.y >= 700)
		{
			man_destination_rect.y = -60;
		}
		else if (man_destination_rect.y < -60)
		{
			man_destination_rect.y = 700;
		}
	}

	//상자 재등장
	if (count == 0) isGet = false;
}

void Render_Stage1()
{
	//배경
	SDL_RenderCopy(g_renderer, g_texture_stage1, &g_source_rectangle_stage1, &g_destination_rectangle_stage1);

	//탱크
	SDL_RenderCopy(g_renderer, sheet_texture, &tank_source_rect, &tank_destination_rect);

	//상자
	if(count==0)
		SDL_RenderCopy(g_renderer, sheet_texture, &chest_source_rect, &chest_destination_rect);
	
	//캐릭터
	if(isRide==false)
		SDL_RenderCopy(g_renderer, sheet_texture, &man_source_rect, &man_destination_rect);

	//Count Text
	{
		SDL_Rect r;
		r.x = 890;
		r.y = 568;
		r.w = bullet_text_rect.w;
		r.h = bullet_text_rect.h;
		SDL_RenderCopy(g_renderer, bullet_text, 0, &r);
	}

	//Ride Text
	{
		if (isRide == true)
		{
			SDL_Rect r2;
			r2.x = 60;
			r2.y = 600;
			r2.w = ride_text_rect.w;
			r2.h = ride_text_rect.h;
			SDL_RenderCopy(g_renderer, ride_text, 0, &r2);
		}
	}

	//대포알 그리기
	for (int i = count; i < 6; i++)
		if (rocket[i].isFire == 1) Fire(i);

	SDL_RenderPresent(g_renderer); // draw to the screen
}

void HandleEvents_Stage1()
{
	Uint8* keysArray;
	keysArray = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));

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

			if (event.key.keysym.sym == SDLK_SPACE)
			{
				//상자와 캐릭터가 겹치면 아이템 획득 가능
				if (isGet == false)
				{
					if (man_destination_rect.x + 21 > chest_destination_rect.x &&
						man_destination_rect.y + 21 > chest_destination_rect.y &&
						man_destination_rect.x+20 < chest_destination_rect.x + chest_destination_rect.w &&
						man_destination_rect.y+20 < chest_destination_rect.y + chest_destination_rect.h)
					{
						isGet = true;
						count = 5;
						Mix_VolumeChunk(item, 100);
						Mix_PlayChannel(-1, item, 0);
					}
				}
				//탱크와 캐릭터가 겹치면 탑승 가능
				if (isRide == false)
				{
					if (man_destination_rect.x + 20 > tank_destination_rect.x &&
						man_destination_rect.y + 20 > tank_destination_rect.y &&
						man_destination_rect.x < tank_destination_rect.x + tank_destination_rect.w &&
						man_destination_rect.y < tank_destination_rect.y + tank_destination_rect.h)
					{
						isRide = true;
						Mix_VolumeChunk(ride, 100);
						Mix_PlayChannel(-1, ride, 0);
					}
				}
				else
				{
					isRide = false;
					man_destination_rect.x = tank_destination_rect.x+10;
					man_destination_rect.y = tank_destination_rect.y+15;
					Mix_VolumeChunk(ride, 100);
					Mix_PlayChannel(-1, ride, 0);
				}
			}
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				//방향키에 따른 스프라이트 변경
				if (isRide == true)
				{
					tank_state = 0;
					tank_source_rect.x = 555;
					tank_source_rect.y = 149;
					tank_source_rect.w = 75;
					tank_source_rect.h = 64;
					tank_destination_rect.w = tank_source_rect.w;
					tank_destination_rect.h = tank_source_rect.h;
				}
				//대포 발사
				if (count != 0&&isRide==true)
				{
					rocket[count].x = tank_destination_rect.x - 20; //대포알 초기 위치 재설정
					rocket[count].y = tank_destination_rect.y + 22;;
					rocket[count].direction = 0;//대포알 방향
					rocket[count].isFire = 1; //발사 상태
					Mix_VolumeChunk(fire, 100);
					Mix_PlayChannel(-1, fire, 0);
					--count;
				}
			}
			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				if (isRide == true)
				{
					tank_state = 1;
					tank_source_rect.x = 554;
					tank_source_rect.y = 25;
					tank_source_rect.w = 75;
					tank_source_rect.h = 61;
					tank_destination_rect.w = tank_source_rect.w;
					tank_destination_rect.h = tank_source_rect.h;
				}
				if (count != 0 && isRide == true)
				{
					rocket[count].x = tank_destination_rect.x + 80;
					rocket[count].y = tank_destination_rect.y + 22;;
					rocket[count].direction = 1;
					rocket[count].isFire = 1;
					Mix_VolumeChunk(fire, 50);
					Mix_PlayChannel(-1, fire, 0);
					--count;
				}
			}
			if (event.key.keysym.sym == SDLK_UP)
			{
				if (isRide == true)
				{
					tank_state = 2;
					tank_source_rect.x = 386;
					tank_source_rect.y = 17;
					tank_source_rect.w = 64;
					tank_source_rect.h = 77;
					tank_destination_rect.w = tank_source_rect.w;
					tank_destination_rect.h = tank_source_rect.h;
				}
				if (count != 0 && isRide == true)
				{
					rocket[count].x = tank_destination_rect.x + 24;
					rocket[count].y = tank_destination_rect.y - 20;;
					rocket[count].direction = 2;
					rocket[count].isFire = 1;
					Mix_VolumeChunk(fire, 50);
					Mix_PlayChannel(-1, fire, 0);
					--count;
				}
			}
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				if (isRide == true)
				{
					tank_state = 3;
					tank_source_rect.x = 386;
					tank_source_rect.y = 143;
					tank_source_rect.w = 63;
					tank_source_rect.h = 77;
					tank_destination_rect.w = tank_source_rect.w;
					tank_destination_rect.h = tank_source_rect.h;
				}
				if (count != 0 && isRide == true)
				{
					rocket[count].x = tank_destination_rect.x + 24;
					rocket[count].y = tank_destination_rect.y + 80;;
					rocket[count].isFire = 1;
					rocket[count].direction = tank_state;
					Mix_VolumeChunk(fire, 50);
					Mix_PlayChannel(-1, fire, 0);
					--count;

				}
			}
			break;

		case SDL_KEYUP:
			break;

		case SDL_MOUSEBUTTONDOWN:
			// If the mouse left button is pressed. 
			if ( event.button.button == SDL_BUTTON_RIGHT )
			{
				Mix_PlayMusic(ending_music, 1);
				Mix_VolumeMusic(20);
				g_current_game_phase = PHASE_ENDING;
			}
			break;

		default:
			break;
		}
	}

#pragma region 캐릭터 이동은 keysArray로 구현
	//이동 기능과 이동 방향에 따른 스프라이트 변경
	if (isRide == false)
	{
		if (keysArray[SDL_SCANCODE_LEFT])
			{
				man_destination_rect.x -= 10;
				man_source_rect.x = 1;
				man_source_rect.y = 143;
				man_source_rect.w = 65;
				man_source_rect.h = 66;
			}
		if (keysArray[SDL_SCANCODE_RIGHT])
			{
				man_destination_rect.x += 10;
				man_source_rect.x = 2;
				man_source_rect.y = 431;
				man_source_rect.w = 59;
				man_source_rect.h = 66;
			}
		if (keysArray[SDL_SCANCODE_UP])
			{
				man_destination_rect.y -= 10;
				man_source_rect.x = 0;
				man_source_rect.y = 289;
				man_source_rect.w = 66;
				man_source_rect.h = 60;
			}
		if (keysArray[SDL_SCANCODE_DOWN])
			{
				man_destination_rect.y += 10;
				man_source_rect.x = 0;
				man_source_rect.y = 0;
				man_source_rect.w = 66;
				man_source_rect.h = 60;
			}
		if (keysArray[SDL_SCANCODE_UP] && keysArray[SDL_SCANCODE_LEFT])
			{
				man_source_rect.x = 0;
				man_source_rect.y = 214;
				man_source_rect.w = 64;
				man_source_rect.h = 68;
			}
		if (keysArray[SDL_SCANCODE_UP] && keysArray[SDL_SCANCODE_RIGHT])
			{
				man_source_rect.x = 1;
				man_source_rect.y = 360;
				man_source_rect.w = 65;
				man_source_rect.h = 64;
			}
		if (keysArray[SDL_SCANCODE_DOWN] && keysArray[SDL_SCANCODE_LEFT])
			{
				man_source_rect.x = 149;
				man_source_rect.y = 70;
				man_source_rect.w = 62;
				man_source_rect.h = 59;
			}
		if (keysArray[SDL_SCANCODE_DOWN] && keysArray[SDL_SCANCODE_RIGHT])
			{
				man_source_rect.x = 2;
				man_source_rect.y = 501;
				man_source_rect.w = 61;
				man_source_rect.h = 67;
			}
	}
#pragma endregion

}

void Clear_Stage1()
{
	SDL_DestroyTexture(g_texture_stage1);
	SDL_DestroyTexture(sheet_texture);
	if (ending_music) Mix_FreeMusic(ending_music);
	if (ride) Mix_FreeChunk(ride);
	if (fire) Mix_FreeChunk(fire);
	if (item) Mix_FreeChunk(item);
	TTF_CloseFont(game_font);
}

