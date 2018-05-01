
#include "Header.h"
#include "TimerClass.h"
#include "Object.h"



const char *title = "test";
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTexture = NULL;
SDL_Surface *gSurface = NULL;
SDL_Joystick *gGameJoystick = NULL;

#ifdef _SDL_TTF_H
	TTF_Font *gFont = NULL;
#endif // _SDL_TTF_H


//The music that will be played
Mix_Music *gMusic = NULL;

//The sound effects that will be used
Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;

LTexture gArrowTexture;
LTexture gTextTexture;
LTexture gButtonSpriteSheeTexture;

LTexture gDownTexture;
LTexture gRightTexture;
LTexture gUpTexture;
LTexture gLeftTexture;
LTexture gPressTexture;
LTexture gTimerStartTexture;
LTexture gTimerPauseTexture;
LTexture gTextTimeTexture;

LTexture g0bjTexture;

//Button class
LButton gButtons[BUTTON_SPRITE_TOTAL];

SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

SDL_Texture *LoadTexture(string path);

int main(int argc, char* argv[])
{
	//初始化应用。
	if (!init())
	{
		cout << "Failed to initialize." << endl;
		_getch();
		return -1;
	}
	//加载媒体
	if (!loadMedia())
	{
		cout << "Failed to LoadMedia." << endl;
		_getch();
		return -1;
	}

	double degrees = 0;
	SDL_RendererFlip fliptype = SDL_FLIP_NONE;
	SDL_Event Event;

	SDL_Color textColor = { 255, 0, 0, 255 };

	//Current time start time
	Uint32 startTime = 0;

	LTimer fpsTimer;

	LTimer capTimer;

	//In memory text stream
	stringstream TimeText;

	//Current rendered texture
	LTexture* currentTexture = NULL;
	bool quit = false;

	//Normalized direction
	int xDir = 0;
	int yDir = 0;

	//Current input device flag
	int CurrentInputDeviceFlag = 0;

	Object obj(&g0bjTexture);
	//Object other(&g0bjTexture,gWin_width / 4, gWin_height / 4);


	SDL_Rect wall;
	wall.x = 300;
	wall.y = 40;
	wall.w = 40;
	wall.h = 400;

	SDL_Rect camera = { 0,0,gWin_width,gWin_height };

	SDL_Color TextColor = { 0,0,0,0xff };

	string inputText = "Some Text";
	gTextTexture.loadFromRenderedText(inputText.c_str(), TextColor);

	int scrollingOffset = 0;

	int counted = 0;
	fpsTimer.start();

	SDL_StartTextInput();
	while (!quit)
	{
		capTimer.start();

		bool renderText = false;

		//Calculate and correct fps
		float avgFPS = counted / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 2000000)
		{
			avgFPS = 0;
		}

		while (SDL_PollEvent(&Event))
		{
			if (Event.type == SDL_QUIT)
			{
				quit = true;
			}else
				if (Event.type == SDL_KEYDOWN)
				{
					switch (Event.key.keysym.sym)
					{
					case SDLK_a:
						degrees -= 60;
						break;
					case SDLK_d:
						degrees += 60;
						break;
					case SDLK_q:
						fliptype = SDL_FLIP_HORIZONTAL;
						break;
					case SDLK_w:
						fliptype = SDL_FLIP_NONE;
						break;
					case SDLK_e:
						fliptype = SDL_FLIP_VERTICAL;
						break;
					case SDLK_ESCAPE:
						quit = true;
						break;
					case SDLK_1:
						Mix_PlayChannel(-1, gHigh, 0);
						break;	
					case SDLK_2:
						Mix_PlayChannel(-1, gMedium, 0);
						break;
					case SDLK_3:
						Mix_PlayChannel(-1, gLow, 0);
						break;
					case SDLK_4:
						Mix_PlayChannel(-1, gScratch, 0);
						break;

					case SDLK_9:
						if (Mix_PlayingMusic() == 0)
							Mix_PlayMusic(gMusic, -1);
						else
							if (Mix_PausedMusic() == 1)
								Mix_ResumeMusic();
							else
								Mix_PauseMusic();

					case SDLK_0:
						Mix_HaltMusic();
						break;

					case SDLK_RETURN:
						startTime = SDL_GetTicks();
						break;
					case SDLK_k:
						if (fpsTimer.isStarted())
							fpsTimer.stop();
						else
							fpsTimer.start();
						break;
					case SDLK_l:
						if (fpsTimer.isPause())
							fpsTimer.unpause();
						else
							fpsTimer.pause();
						break;

					default:
						
						break;
					}
					CurrentInputDeviceFlag = KEYBOARD;
				}
				else
					if (Event.type == SDL_JOYAXISMOTION)
					{
						//Motion on controller 0
						if (Event.jaxis.which == 0)
						{
							//X axis motion
							if (Event.jaxis.axis == 0)
							{
								//Left of dead zone
								if (Event.jaxis.value < -JOYSTICK_DEAD_ZONE)
								{
									xDir = -1;
								}
								//Right of dead zone
								else if (Event.jaxis.value > JOYSTICK_DEAD_ZONE)
								{
									xDir = 1;
								}
								else
								{
									xDir = 0;
								}
							}
							//Y axis motion
							else
							{
								//Below of dead zone
								if (Event.jaxis.value < -JOYSTICK_DEAD_ZONE)
								{
									yDir = -1;
								}
								//Above of dead zone
								else if (Event.jaxis.value > JOYSTICK_DEAD_ZONE)
								{
									yDir = 1;
								}
								else
								{
									yDir = 0;
								}

							}
							
						}
						CurrentInputDeviceFlag = JOSTICK;
					}

					if (Event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
					{
						inputText.pop_back();
						renderText = true;
					}
					else
						if (Event.key.keysym.sym == SDLK_c && SDL_GetModState()&KMOD_CTRL)
							SDL_SetClipboardText(inputText.c_str());
						else
							if (Event.key.keysym.sym == SDLK_v && SDL_GetModState()&KMOD_CTRL)
							{
								inputText = SDL_GetClipboardText();
								renderText = true;
							}
							else
								if(Event.type==SDL_TEXTINPUT)
									if (!((Event.text.text[0] == 'c' || Event.text.text[0] == 'C')
										&& (Event.text.text[0] == 'v' || Event.text.text[0] == 'V')
										&& (SDL_GetModState()&KMOD_CTRL)
										))
									{
										inputText += Event.text.text;
										renderText = true;
									}
					

			//Handle button events
			for (int i = 0; i < TOTAL_BUTTONS; ++i)
			{
				gButtons[i].handleEvent(&Event);
			}

			obj.HandleEvent(Event);

		}
		
		//Key board controller
		const Uint8 *CurrentKeyStates = SDL_GetKeyboardState(NULL);
		if (CurrentKeyStates[SDL_SCANCODE_UP])
		{
			currentTexture = &gUpTexture;
		}
		else if (CurrentKeyStates[SDL_SCANCODE_DOWN])
		{
			currentTexture = &gDownTexture;
		}
		else if (CurrentKeyStates[SDL_SCANCODE_LEFT])
		{
			currentTexture = &gLeftTexture;
		}
		else if (CurrentKeyStates[SDL_SCANCODE_RIGHT])
		{
			currentTexture = &gRightTexture;
		}
		else
		{
			currentTexture = &gPressTexture;
		}

		if (renderText)
		{
			if (inputText != "")
				gTextTexture.loadFromRenderedText(inputText.c_str(), TextColor);
			else
				gTextTexture.loadFromRenderedText(" ", TextColor);
		}


		// Count area
		int x = (gWin_width - gArrowTexture.getWidht()) / 2;
		int y = (gWin_height - gArrowTexture.getHeight()) / 2;
		

		SDL_Rect rect;
		rect.x = 100;
		rect.y = 100;
		rect.w = 200;
		rect.h = 100;

		//Calculate angle
		//if (CurrentInputDeviceFlag == JOSTICK)
		//{
		//	degrees = atan2((double)yDir, (double)xDir) * (180.0 / M_PI);

		//	//Correct angle
		//	if (xDir == 0 && yDir == 0)
		//	{
		//		degrees = 0;
		//	}
		//}
		//Set text to be rendered
		/*TimeText.str("");
		TimeText << "Average Frames Per Second " << avgFPS<<"fps";*/

		
		/*if (!gTextTimeTexture.loadFromRenderedText(TimeText.str().c_str(), textColor))
		{
			cout << "Unable to render time texture"<<endl;
		}*/
		

		obj.move();
		 
		//Center the camera over the dot
		camera.x = (obj.getPosX() + Object::object_widht / 2) - gWin_width / 2;
		camera.y = (obj.getPosY() + Object::object_height / 2) - gWin_height / 2;


		//Keep the camera in bounds
		if (camera.x < 0)
		{
			camera.x = 0;
		}
		if (camera.y < 0)
		{
			camera.y = 0;
		}
		if (camera.x > LEVEL_WIDTH - camera.w)
		{
			camera.x = LEVEL_WIDTH - camera.w;
		}
		if (camera.y > LEVEL_HEIGHT - camera.h)
		{
			camera.y = LEVEL_HEIGHT - camera.h;
		}


		//scroll background
		--scrollingOffset;
		if (scrollingOffset < -gArrowTexture.getWidht())
		{
			scrollingOffset = 0;
		}

		//clear device
		SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(gRenderer);
		

		//Renderer area

		//SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0x0, 0xff);
		//SDL_RenderDrawRect(gRenderer, &wall);
		
		//gArrowTexture.Render(scrollingOffset,0);
		//gArrowTexture.Render(scrollingOffset + gArrowTexture.getWidht(), 0);

		gTextTexture.Render((gWin_width - gTextTexture.getWidht()) / 2, 240);

		obj.Render(camera.x,camera.y);
		//other.Render();

		//SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0xff);
		//SDL_RenderFillRect(gRenderer, &rect);

		//gPromptTexture.Render(x, y, NULL,degrees,NULL,fliptype);
		/*int x_text = (gWin_width - gTextTexture.getWidht()) / 2;
		int y_text = 0;

		gTextTexture.Render(x_text, y_text);

		int x_start = (gWin_width - gTextTexture.getWidht()) / 2;
		int y_start = gTextTexture.getHeight() + 10;
		gTimerStartTexture.Render(x_start, y_start);

		int x_pause = (gWin_width - gTextTexture.getWidht()) / 2;
		int y_pause = gTimerStartTexture.getHeight() + 50;
		gTimerPauseTexture.Render(x_pause, y_pause);*/

		/*int x_timetext = (gWin_width - gTextTimeTexture.getWidht()) / 2;
		int y_timetext = (gWin_height - gTextTimeTexture.getHeight()) / 2;

		gTextTimeTexture.Render(x_timetext, y_timetext);*/
		//gTextTexture.Render(0, 240);
		////Render buttons
		//for (int i = 0; i < TOTAL_BUTTONS; ++i)
		//{
		//gButtons[i].render();
		//gButtons[i].setPosition(x, y);
		//}
		//currentTexture->Render(x, y);
		
		
		SDL_RenderPresent(gRenderer);
		++counted;

		int frameTicks = capTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
		//SDL_UpdateWindowSurface(gWindow);
	}
	SDL_StopTextInput();
	

	//释放所有
	close();
	return 0;
}

bool init()
{
	//子系统初始化：定时器子系统、音频子系统、视频子系统、自动初始化事件子系统、操纵杆子系统、触觉子系统、事件子系统
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "Unble to initialize SDL:" << SDL_GetError() << endl;
		return false;
	}

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		cout << "Waring:Linear texture filtering not enabled!"<<endl;
	}


	//Check for joystick 
	if (SDL_NumJoysticks() < 1)
	{

		cout << "Waring:No joysticks connected!" << endl;
	}
	else
	{
		// Load joystick
		gGameJoystick = SDL_JoystickOpen(0);
		if (gGameJoystick == NULL)
		{
			cout << "Waring:Unable ti open game controller! Error:" << SDL_GetError()<<endl;
			return false;
		}

	}
	//创建窗口
	gWindow = SDL_CreateWindow(title, gWin_PosX, gWin_PosY, gWin_width, gWin_height, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		cout << "Could not create window:" << SDL_GetError() << endl;
		return false;
	}
	//创建Renderer
	Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
	gRenderer = SDL_CreateRenderer(gWindow, -1, flags);
	if (gRenderer == NULL)
	{
		cout << "Could not create Renderer:" << SDL_GetError();
		return false;
	}
	else
	{
		SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);

		//initialize IMAGE
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags)&imgFlags)) {
			cout << "SDL_image could not initialize! Error:" << IMG_GetError()<<endl;
			return false;
		}
#ifdef _SDL_TTF_H
		if (TTF_Init() == -1)
		{
			cout << "TTF_Init could not initialize! Error:" << TTF_GetError() << endl;
			return false;
		}
#endif // _SDL_TTF_H

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)<0)
		{
			cout << "SDL_mixer could not initialize! Error:" << Mix_GetError()<<endl;
			return false;
		}

		
	}

	gArrowTexture.setRenderer(gRenderer);
	gButtonSpriteSheeTexture.setRenderer(gRenderer);
	gUpTexture.setRenderer(gRenderer);
	gDownTexture.setRenderer(gRenderer);
	gLeftTexture.setRenderer(gRenderer);
	gRightTexture.setRenderer(gRenderer);
	gPressTexture.setRenderer(gRenderer);

	gTimerStartTexture.setRenderer(gRenderer);
	gTimerPauseTexture.setRenderer(gRenderer);
	gTextTimeTexture.setRenderer(gRenderer);
	gTextTexture.setRenderer(gRenderer);
	
	g0bjTexture.setRenderer(gRenderer);
	return true;
}


bool loadMedia() 
{
	if (!gButtonSpriteSheeTexture.loadFromFile("src/images/button.png"))
	{
		cout << "Failed to loadFromFile" << endl;
		return false;
	}

	for (int i = 0; i < BUTTON_SPRITE_TOTAL; i++)
	{
		gSpriteClips[i].x = 0;
		gSpriteClips[i].y = 0 + i * BUTTON_HEIGHT;
		gSpriteClips[i].w = BUTTON_WIDTH;
		gSpriteClips[i].h = BUTTON_HEIGHT;
	}
	
	for (int i = 0; i < BUTTON_SPRITE_TOTAL; i++)
	{
		
		gButtons[i].setButtonSize(gSpriteClips);
		gButtons[i].setLTexture(gButtonSpriteSheeTexture);
		
	}

	if (!gUpTexture.loadFromFile("src/images/up.png"))
	{
		cout << "Failed to loadFromFile" << endl;
		return false;
	}

	if (!gDownTexture.loadFromFile("src/images/down.png"))
	{
		cout << "Failed to loadFromFile" << endl;
		return false;
	}

	if (!gLeftTexture.loadFromFile("src/images/left.png"))
	{
		cout << "Failed to loadFromFile" << endl;
		return false;
	}

	if (!gRightTexture.loadFromFile("src/images/right.png"))
	{
		cout << "Failed to loadFromFile" << endl;
		return false;
	}

	if (!gPressTexture.loadFromFile("src/images/press.png"))
	{
		cout << "Failed to loadFromFile" << endl;
		return false;
	}
	if (!gArrowTexture.loadFromFile("src/images/bg.png"))
	{
		cout << "Failed to loadFromFile" << endl;
		return false;
	}

	

	if (!g0bjTexture.loadFromFile("src/images/dot.bmp"))
	{
		cout << "failed to loadframfile" << endl;
		return false;
	}

	//Load prompt texture
	/*if (!gTimerStartTexture.loadFromFile("src/images/prompt.png"))
	{
		printf("Failed to load prompt texture!\n");
		return false;
	}*/

	//Load music
	gMusic = Mix_LoadMUS("src/Audio/beat.wav");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	//Load sound effects
	gScratch = Mix_LoadWAV("src/Audio/scratch.wav");
	if (gScratch == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	gHigh = Mix_LoadWAV("src/Audio/high.wav");
	if (gHigh == NULL)
	{
		printf("Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	gMedium = Mix_LoadWAV("src/Audio/medium.wav");
	if (gMedium == NULL)
	{
		printf("Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	gLow = Mix_LoadWAV("src/Audio/low.wav");
	if (gLow == NULL)
	{
		printf("Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}


#ifdef _SDL_TTF_H
	gFont = TTF_OpenFont("src/Fonts/lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}
	else
	{
		gTimerPauseTexture.setFont(gFont);
		gTimerStartTexture.setFont(gFont);
		gTextTimeTexture.setFont(gFont);
		gTextTexture.setFont(gFont);
		//Render text
		SDL_Color textColor = { 255, 0, 0,255 };
		if (!gTextTexture.loadFromRenderedText("Press Enter to Reset Start Time.", textColor))
		{
			printf("Failed to render text texture!\n");
			return false;
		}
		if (!gTimerStartTexture.loadFromRenderedText("Press K to start timer", textColor))
		{
			printf("Failed to render text texture!\n");
			return false;
		}

		if (!gTimerPauseTexture.loadFromRenderedText("Press L to puase tiemr", textColor))
		{
			cout << "Failed to render text texture!" << endl;
			return false;
		}
		
	}
#endif // _SDL_TTF_H
	return true;
}

bool checkCollision(vector<SDL_Rect>& obj_a, vector<SDL_Rect>& obj_b)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	for (int Abox = 0; Abox < obj_a.size(); Abox++) {
		leftA = obj_a[Abox].x;
		rightA = obj_a[Abox].x + obj_a[Abox].w;
		topA = obj_a[Abox].y;
		bottomA = obj_a[Abox].y + obj_a[Abox].h;

		for (int Bbox = 0; Bbox < obj_b.size(); Bbox++) {
			leftB = obj_b[Bbox].x;
			rightB = obj_b[Bbox].x + obj_b[Bbox].w;
			topB = obj_b[Bbox].y;
			bottomB = obj_b[Bbox].y + obj_b[Bbox].h;

			if ((bottomA <= topB || topA >= bottomB || leftA >= rightB || rightA <= leftB)==false)
			{
				return true;
			}

		}
	}
	


	return false;
}

bool checkCollision(Circle& obj_a, Circle& obj_b)
{
	//Calculate total radius squared
	int totalRadiusSquared = obj_a.r + obj_b.r;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

	//If the distance between the centers of the circles is less than the sum of their radii
	if (distanceSquared(obj_a.x, obj_a.y, obj_b.x, obj_b.y) < (totalRadiusSquared))
	{
		//The circles have collided
		return true;
	}

	//If not
	return false;
}

bool checkCollision(Circle &obj_a, SDL_Rect &obj_b)
{
	//Closest point on collision box
	int cX, cY;

	//Find closest x offset
	if (obj_a.x < obj_b.x)
	{
		cX = obj_b.x;
	}
	else if (obj_a.x > obj_b.x + obj_b.w)
	{
		cX = obj_b.x + obj_b.w;
	}
	else
	{
		cX = obj_a.x;
	}

	//Find closest y offset
	if (obj_a.y < obj_b.y)
	{
		cY = obj_b.y;
	}
	else if (obj_a.y > obj_b.y + obj_b.h)
	{
		cY = obj_b.y + obj_b.h;
	}
	else
	{
		cY = obj_a.y;
	}

	//If the closest point is inside the circle
	if (distanceSquared(obj_a.x, obj_a.y, cX, cY) < obj_a.r * obj_a.r)
	{
		//This box and the circle have collided
		return true;
	}

	//If the shapes have not collided
	return false;
}

double distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX * deltaX + deltaY * deltaY;
}

void close()
{

	g0bjTexture.free();

	gTimerStartTexture.free();


	//Free the sound effects
	Mix_FreeChunk(gScratch);
	Mix_FreeChunk(gHigh);
	Mix_FreeChunk(gMedium);
	Mix_FreeChunk(gLow);
	gScratch = NULL;
	gHigh = NULL;
	gMedium = NULL;
	gLow = NULL;

	//Free the music
	Mix_FreeMusic(gMusic);
	gMusic = NULL;

	gArrowTexture.free();

	gTextTexture.free();

	gButtonSpriteSheeTexture.free();

	SDL_JoystickClose(gGameJoystick);
	gGameJoystick = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


SDL_Texture *LoadTexture(string path)
{
	SDL_Texture *newTexture = NULL;

	SDL_Surface *tempSurface = IMG_Load(path.c_str());
	if (tempSurface == NULL)
	{
		cout << "Failed to IMG_Load:" << IMG_GetError() << endl;
		return NULL;
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
		if (newTexture == NULL)
		{
			cout << "Unable to create texture from:" << path.c_str() << "Error:" << IMG_GetError() << endl;
			return NULL;
		}
		SDL_FreeSurface(tempSurface);
	}

	return newTexture;
}