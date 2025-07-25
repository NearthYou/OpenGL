
#include "Ball.h"
#include "SDL_image.h"
#include <iostream>
#include "ProjectileGame.h"



Ball::Ball(double radius, Room *room)
{
	room_ = room;
	radius_ = radius;

	
	v_[0] = 0;
	v_[1] = 0;

	mass_ = 2; // 2kg
	coeff_of_restitution_ = 0.7;
	coeff_of_friction_ = 0.96;

	Reset();
}

void
Ball::Reset()
{
	p_[0] = radius_ + room_->left_wall_x();
	p_[1] = radius_;

	v_[0] = 0;
	v_[1] = 0;
}

void
Ball::Launch(double initial_force_x, double initial_force_y)
{
	v_[0] = v_[0] + (initial_force_x/mass_);
	v_[1] = v_[1] + (initial_force_y/mass_);
}



void 
Ball::Update(double timestep_s)
{
	double dt = timestep_s;	// seconds



	// 가속도
	double a[2];
	a[0] = 0;
	a[1] = room_->gravitational_acc_y();// Gravity

	// Move
	p_[0] = p_[0] + dt * v_[0];
	p_[1] = p_[1] + dt * v_[1];


	// Collision with Ground
	if ( p_[1]-radius_ < room_->ground_height() && v_[1] < 0 ) 
	{
		p_[1] = radius_ + room_->ground_height();

		v_[1] = -1*v_[1];

		// Coefficient of restitution
		v_[1] = coeff_of_restitution_ * v_[1];
		v_[0] *= coeff_of_friction_;
	}

	// Collision with Ceilling
	else if ( p_[1]+radius_ > room_->height() && v_[1] > 0 ) 
	{
		p_[1] = room_->height() - radius_;

		v_[1] = -1*v_[1];

		// Coefficient of restitution
		v_[1] = coeff_of_restitution_ * v_[1];
	}

	// Collision with Left Wall
	if ( p_[0] - radius_ < room_->left_wall_x() && v_[0] < 0 )
	{
		p_[0] = room_->left_wall_x() + radius_;

		v_[0] = -1 * v_[0];

		// Coefficient of restitution
		v_[0] = coeff_of_restitution_ * v_[0];
	}

	// Collision with Right Wall
	else if (p_[0] + radius_ > room_->right_wall_x() && v_[0] > 0)
	{
		p_[0] = room_->right_wall_x() - radius_;

		v_[0] = -1 * v_[0];

		// Coefficient of restitution
		v_[0] = coeff_of_restitution_ * v_[0];
	}

	// Collision with Net-Left
	if (p_[0] + radius_ > room_->vertiacal_fence_pos_x() && v_[0] > 0 && p_[0] + radius_ < room_->vertiacal_fence_pos_x() + 0.1
		&& p_[1] + radius_ < room_->vertiacal_fence_height() + 0.17)
	{

			p_[0] = room_->vertiacal_fence_pos_x() - radius_;
			v_[0] = -1 * v_[0];
			// Coefficient of restitution
			v_[0] = coeff_of_restitution_ * v_[0];
	}

	// Collision with Net-Right
	if (p_[0] + radius_ > room_->vertiacal_fence_pos_x())
	{
		if (p_[0] - radius_ < room_->vertiacal_fence_pos_x() && v_[0] < 0 && p_[0] - radius_ < room_->vertiacal_fence_pos_x() + 0.1
			&& p_[1] + radius_ < room_->vertiacal_fence_height() + 0.17)
		{
			p_[0] = room_->vertiacal_fence_pos_x() + radius_;

			v_[0] = -1 * v_[0];

			// Coefficient of restitution
			v_[0] = coeff_of_restitution_ * v_[0];
		}
	}


	// Collision with Net-Top
	if (p_[1] - radius_ <= room_->vertiacal_fence_height()  && v_[1] < 0 &&
		(p_[0] + radius_ < room_->vertiacal_fence_pos_x()+radius_+0.06)
		&& (p_[0] > room_->vertiacal_fence_pos_x()))
	{
		p_[1] = radius_ + room_->vertiacal_fence_height();

		v_[1] = -1 * v_[1];

		// Coefficient of restitution
		v_[1] = coeff_of_restitution_ * v_[1];
		v_[0] *= coeff_of_friction_;
	}


	//속도 = 이전속도 + 시간(dt) * 가속도;
	v_[0] = v_[0] + dt * a[0];
	v_[1] = v_[1] + dt * a[1];
}
