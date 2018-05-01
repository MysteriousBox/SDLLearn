#include "Object.h"



Object::Object(LTexture *objTexture, int x, int y)
{
	mObjeTexture = objTexture;
	mPos_x = x;
	mPos_y = y;

	mColliders.resize(11);

	mCollider.r = object_widht / 2;

	mVel_x = 0;
	mVel_y = 0;

	//Initialize the collision boxes' width and height
	mColliders[0].w = 6;
	mColliders[0].h = 1;

	mColliders[1].w = 10;
	mColliders[1].h = 1;

	mColliders[2].w = 14;
	mColliders[2].h = 1;

	mColliders[3].w = 16;
	mColliders[3].h = 2;

	mColliders[4].w = 18;
	mColliders[4].h = 2;

	mColliders[5].w = 20;
	mColliders[5].h = 6;

	mColliders[6].w = 18;
	mColliders[6].h = 2;

	mColliders[7].w = 16;
	mColliders[7].h = 2;

	mColliders[8].w = 14;
	mColliders[8].h = 1;

	mColliders[9].w = 10;
	mColliders[9].h = 1;

	mColliders[10].w = 6;
	mColliders[10].h = 1;

	//Initialize colliders relative to position
	shiftColliders();
}


Object::~Object()
{
}

void Object::shiftColliders()
{
	//The row offset
	int r = 0;

	for (int set = 0; set < mColliders.size(); set++)
	{
		//Center the collision box
		mColliders[set].x = mPos_x + (object_widht - mColliders[set].w) / 2;

		//Set the collision box at its row offset
		mColliders[set].y = mPos_y + r;

		//Move the row offset down the height of the collision box
		r += mColliders[set].h;
	}
	mCollider.x = mPos_x;
	mCollider.y = mPos_y;
}

vector<SDL_Rect>& Object::getColliders()
{
	return mColliders;
}

Circle& Object::getCollider()
{
	return mCollider;
}

int  Object::getPosX()
{
	return mPos_x;
}

int Object::getPosY()
{
	return mPos_y;
}

void Object::HandleEvent(SDL_Event &e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
			case SDLK_UP: mVel_y -= obj_vel; break;
			case SDLK_DOWN: mVel_y += obj_vel; break;
			case SDLK_LEFT: mVel_x -= obj_vel; break;
			case SDLK_RIGHT: mVel_x += obj_vel; break;
			default:
				break;
		}
	}
	else
		if (e.type == SDL_KEYUP && e.key.repeat == 0)
		{
			switch (e.key.keysym.sym)
			{
				case SDLK_UP: mVel_y += obj_vel; break;
				case SDLK_DOWN: mVel_y -= obj_vel; break;
				case SDLK_LEFT: mVel_x += obj_vel; break;
				case SDLK_RIGHT: mVel_x -= obj_vel; break;
			default:
				break;
			}
		}
}

void Object::move(vector<SDL_Rect>&otherColliders)
{
	//Move the obj up or right
	mPos_x += mVel_x;
	shiftColliders();

	//If the obj went too far left or right
	if (mPos_x < 0 || mPos_x + object_widht>gWin_width||checkCollision(mColliders,otherColliders))
	{
		mPos_x -= mVel_x;
		shiftColliders();
	}
	//Move the obj up or down
	mPos_y += mVel_y;
	shiftColliders();

	//If the obj went too far up or down
	if ((mPos_y < 0) || (mPos_y + object_height > gWin_height)||checkCollision(mColliders,otherColliders))
	{
		//Move back
		mPos_y -= mVel_y;
		shiftColliders();
	}
}

void Object::move(SDL_Rect &square, Circle &circle)
{
	//Move the dot left or right
	mPos_x += mVel_x;
	shiftColliders();

	//If the dot collided or went too far to the left or right
	if ((mPos_x - mCollider.r < 0) || (mPos_x + mCollider.r > gWin_width) || checkCollision(mCollider, square) || checkCollision(mCollider, circle))
	{
		//Move back
		mPos_x -= mVel_x;
		shiftColliders();
	}

	//Move the dot up or down
	mPos_y += mVel_y;
	shiftColliders();

	//If the dot collided or went too far up or down
	if ((mPos_y - mCollider.r < 0) || (mPos_y + mCollider.r > gWin_height) || checkCollision(mCollider, square) || checkCollision(mCollider, circle))
	{
		//Move back
		mPos_y -= mVel_y;
		shiftColliders();
	}

}

void Object::move()
{
	//Move the dot left or right
	mPos_x += mVel_x;

	//If the dot went too far to the left or right
	if ((mPos_x < 0) || (mPos_x + object_widht > LEVEL_WIDTH))
	{
		//Move back
		mPos_x -= mVel_x;
	}

	//Move the dot up or down
	mPos_y += mVel_y;

	//If the dot went too far up or down
	if ((mPos_y < 0) || (mPos_y + object_height > LEVEL_HEIGHT))
	{
		//Move back
		mPos_y -= mVel_y;
	}
}

void Object::Render()
{
	mObjeTexture->Render(mPos_x-mCollider.r, mPos_y-mCollider.r);
}

void Object::Render(int camX, int camY)
{
	//Show the dot relative to the camera
	mObjeTexture->Render(mPos_x - camX, mPos_y - camY);
}