#include <iostream>

#include "ProjectileGame.h" 
#include "SDL_image.h"
#include "G2W.h"
#include "math.h"

extern int g_current_game_phase;
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
extern float g_timestep_s;



ProjectileGame::ProjectileGame()
{
	g_flag_running = true;


	// Texture
	{
		SDL_Surface* ball_surface = IMG_Load("../../Resources/ball.png");
		ball_src_rectangle_.x = 0;
		ball_src_rectangle_.y = 0;
		ball_src_rectangle_.w = ball_surface->w;
		ball_src_rectangle_.h = ball_surface->h;

		ball_texture_ = SDL_CreateTextureFromSurface(g_renderer, ball_surface);
		SDL_FreeSurface(ball_surface);
	}




	// Initialize ball pointers
	for ( int i=0; i<MAX_BALL_NUM; i++ )
	{
		balls_[i] = 0;
	}

	num_of_balls_ = 0;

	current_mouse_win_x_ = 0;
	current_mouse_win_y_ = 0;
	before_mouse_win_x_ = current_mouse_win_x_;
	before_mouse_win_y_ = before_mouse_win_y_;
	shoot = false;


	timer = 0;
	count = 0;
	num = 0;

	for (int i = 0; i < 10; i++)
	{
		route[i].x = 0; 
		route[i].y = 0;
	}

	AddNewBall();
}



ProjectileGame::~ProjectileGame()
{
	// Delete balls
	for ( int i=0; i<MAX_BALL_NUM; i++ )
	{
		if ( balls_[i] != 0 )
			delete balls_[i];

		balls_[i] = 0;
	}

	num_of_balls_ = 0;

	SDL_DestroyTexture(ball_texture_);
}


void
ProjectileGame::AddNewBall()
{
	if ( num_of_balls_ == MAX_BALL_NUM ) return;

	// Create new Ball
	Ball *ball = new Ball(0.11f, &room_);

	// Add to the list
	balls_[num_of_balls_] = ball;

	// Increase Num
	num_of_balls_++;

}


void
ProjectileGame::Update()
{
	// Update balls
	for ( int i=0; i<num_of_balls_; i++ )
	{
		balls_[i]->Update(g_timestep_s);
	}

}

void 
ProjectileGame::Render()
{
	SDL_SetRenderDrawColor(g_renderer, 255,255,255,255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color
	
	
	// Draw room_
	{
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);

		// Left Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x()),
										G2W_Y(0), 
										G2W_X(room_.left_wall_x()),
										G2W_Y(room_.height()) );

		
		// Right Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.right_wall_x()),
										G2W_Y(0), 
										G2W_X(room_.right_wall_x()),
										G2W_Y(room_.height()) );

		// Top Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x()),
										G2W_Y(room_.height()), 
										G2W_X(room_.right_wall_x()),
										G2W_Y(room_.height()) );

		// Bottom Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x()),
										G2W_Y(0), 
										G2W_X(room_.right_wall_x()),
										G2W_Y(0) );

		// Fence
		SDL_RenderDrawLine(g_renderer, G2W_X( room_.vertiacal_fence_pos_x() ), 
										G2W_Y(0), 
										G2W_X( room_.vertiacal_fence_pos_x()  ),
										G2W_Y( room_.ground_height()+room_.vertiacal_fence_height() ) );
	}


	// Draw Balls
	for ( int i=0; i<num_of_balls_; i++ )
	{
		Ball *ball = balls_[i];

		int ball_win_x = G2W_X(balls_[i]->pos_x());
		int ball_win_y = G2W_Y(balls_[i]->pos_y());

		double win_radius = G2W_Scale * balls_[i]->radius();

		SDL_Rect dest_rect;
		dest_rect.w = (int)(2*win_radius);
		dest_rect.h = (int)(2*win_radius);
		dest_rect.x = (int)(ball_win_x - win_radius);
		dest_rect.y = (int)(ball_win_y - win_radius);

		SDL_RenderCopy(g_renderer, ball_texture_, &ball_src_rectangle_, &dest_rect);
	}

	
	// Draw the Guide Line 
	if ( num_of_balls_ > 0 )
	{
		SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 100);
		SDL_RenderDrawLine(g_renderer, G2W_X(balls_[num_of_balls_-1]->pos_x()), 
										G2W_Y(balls_[num_of_balls_-1]->pos_y()), 
										current_mouse_win_x_, 
										current_mouse_win_y_ );
	}

	// Expected Route
	
	if(!shoot)
	{
		SDL_SetRenderDrawColor(g_renderer, 0, 255, 255, 255);
		for (int i = 0; i < 10; i++)
		{
			SDL_Rect r;
			r.x = route[i].x;
			r.y = route[i].y;
			r.w = G2W_Scale*0.07;
			r.h = G2W_Scale*0.07;
			SDL_RenderFillRect(g_renderer, &r);
		}
	}


	SDL_RenderPresent(g_renderer); // draw to the screen
}



void 
ProjectileGame::HandleEvents()
{
	SDL_Event event;
	if(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
		
			// If the mouse left button is pressed. 
			if ( event.button.button == SDL_BUTTON_LEFT )
			{
				shoot = true;
				// Get the cursor's x position.
				current_mouse_win_x_ = event.button.x;
				current_mouse_win_y_ = event.button.y;

				
				double mouse_game_x = W2G_X(current_mouse_win_x_);
				double mouse_game_y = W2G_Y(current_mouse_win_y_);
				
				
				// Luanch
				if ( num_of_balls_ > 0 )
				{
					Ball *ball = balls_[num_of_balls_-1];

					// Guide Line Vector
					double guide_line_x = mouse_game_x - ball->pos_x();
					double guide_line_y = mouse_game_y - ball->pos_y();


					// Lauching Force
					double launcing_force_x = 8.0 * guide_line_x;
					double launcing_force_y = 8.0 * guide_line_y;

					// Launch
					balls_[num_of_balls_-1]->Launch(launcing_force_x, launcing_force_y);

					

					// Add a new ball for the next
					AddNewBall();
				
				}


				
				shoot = false;
			}
		

		case SDL_MOUSEMOTION:
			{
				// 마우스의 현재 위치
				current_mouse_win_x_ = event.button.x;
				current_mouse_win_y_ = event.button.y;
				
				double mouse_game_x = W2G_X(current_mouse_win_x_);
				double mouse_game_y = W2G_Y(current_mouse_win_y_);


				if (!shoot)
				{
					Ball* ball = new Ball(0.11f, &room_);

					// Guide Line Vector
					double guide_line_x = mouse_game_x - ball->pos_x();
					double guide_line_y = mouse_game_y - ball->pos_y();

					// Lauching Force
					double launcing_force_x = 8.0 * guide_line_x;
					double launcing_force_y = 8.0 * guide_line_y;


					ball->Launch(launcing_force_x, launcing_force_y);
					
					while (timer <= 1)
					{
						timer += g_timestep_s;
						

						ball->Update(g_timestep_s);

						if ((int)(timer * 10.f) == num)
						{
							route[num].x = G2W_X(ball->pos_x());
							route[num].y = G2W_Y(ball->pos_y());
							num++;
						}
						
					}


					num = 0;
					timer = 0;
					delete(ball);


				}
				
			}
			break;

		case SDL_KEYDOWN:

			if (event.key.keysym.sym == SDLK_1)
			{
				G2W_Scale = 300.f;
				W2G_Scale = 1.f / 300.0f;

			}

			if (event.key.keysym.sym == SDLK_2)
			{
				G2W_Scale = 150.f;
				W2G_Scale = 1.f / 150.0f;
			}
			break;
		default:
			break;
		}
	}
}
