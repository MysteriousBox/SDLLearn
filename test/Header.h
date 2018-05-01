#pragma once
#include <conio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <sstream>
#include <vector>

using namespace std;

const int gWin_width = 640;
const int gWin_height = 480;
const int gWin_PosX = SDL_WINDOWPOS_UNDEFINED;
const int gWin_PosY = SDL_WINDOWPOS_UNDEFINED;
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;
const int JOYSTICK_DEAD_ZONE = 8000;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;



enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

enum InputDevice
{
	KEYBOARD,
	MOUSE,
	JOSTICK
};

struct Circle
{
	int x;
	int y;
	int r;
};

bool init();
bool loadMedia();
void close();
bool checkCollision(vector<SDL_Rect>& obj_a, vector<SDL_Rect>& obj_b);
bool checkCollision(Circle &obj_a, Circle &obj_b);
bool checkCollision(Circle &obj_a, SDL_Rect &obj_b);
double distanceSquared(int x1, int y1, int x2, int y2);

class LTexture
{
public:
	LTexture();
	~LTexture();
	bool loadFromFile(string path);
#ifdef _SDL_TTF_H
	//Create image from font string
	bool loadFromRenderedText(string path, SDL_Color textcolor);
	void setFont(TTF_Font *Font);
#endif // _SDL_TTF_H

	
	void free();
	
	void setRenderer(SDL_Renderer *renderer);
	void setColor(Uint8 red, Uint8 green, Uint8 bule);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);

	void Render(int x, int y, SDL_Rect* clip = NULL, 
		double angle = 0.0, SDL_Point *center = NULL,
		SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidht();
	int getHeight();

private:
	SDL_Texture *mTexture;
	
 	static SDL_Renderer *mRenderer;

#ifdef _SDL_TTF_H
	TTF_Font *mFont;
#endif // _SDL_TTF_H

	//Image dimensions
	int mWidth;
	int mHeight;
};

class LButton
{
public:
	//Initializes internal variables
	LButton();
	~LButton();

	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	//Shows button sprite
	void render();

	//set LTexture
	void setLTexture(LTexture ButtonSpriteSheetTexture);

	//Set button size
	void setButtonSize(SDL_Rect *SpriteClips);

private:
	//Top left position
	SDL_Point mPosition;
	
	LTexture  mButtonSpriteSheetTexture;
	SDL_Rect *mSpriteClips;
	//Currently used global sprite
	LButtonSprite mCurrentSprite;
};

