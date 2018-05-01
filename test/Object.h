#pragma once
#include "Header.h"

class Object
{
public:
	Object( LTexture *objTexture = NULL, int x = 0, int y = 0);
	~Object();
	static const int object_widht = 20;
	static const int object_height = 20;

	//Maximum axis velocity of the object
	static const int obj_vel = 5;

	//Takes key presses an adjusts the dot's velocity
	void HandleEvent(SDL_Event &e);

	//Moves the object
	void move(vector<SDL_Rect>&otherColliders);

	void move(SDL_Rect &square, Circle &circle);

	void move();

	//Shows the object on  the screen
	void Render();
	void Render(int camX, int camY);

	int getPosX();
	int getPosY();

	vector<SDL_Rect>& getColliders();
	Circle &getCollider();
private:
	//The LTexture
	LTexture *mObjeTexture;

	//The X and Y offsets of the object
	int mPos_x, mPos_y;

	//The velocity
	int mVel_x, mVel_y;

	vector<SDL_Rect> mColliders;

	Circle mCollider;

	void shiftColliders();
};

