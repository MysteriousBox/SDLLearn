#include "Header.h"

LTexture::LTexture()
{
}

LTexture::~LTexture()
{
	this->free();
}

int LTexture::getHeight() 
{
	return mHeight;
}

int LTexture::getWidht() 
{
	return mWidth;
}

void LTexture::setRenderer(SDL_Renderer *renderer) 
{
	mRenderer = renderer;
}



bool LTexture::loadFromFile(string path)
{
	free();
	SDL_Texture *newTexture = NULL;
	SDL_Surface *LoadSurface = IMG_Load(path.c_str());
	if (LoadSurface == NULL)
	{
		cout << "Failed to IMAGE Error:" << IMG_GetError();
		return false;
	}
	else
	{
		SDL_SetColorKey(LoadSurface, SDL_TRUE,SDL_MapRGB(LoadSurface->format, 0, 0xff, 0xff));
		newTexture = SDL_CreateTextureFromSurface(mRenderer, LoadSurface);
		if (newTexture == NULL)
		{
			cout << "Unable to create texture Error:" << SDL_GetError() << endl;
			return false;
		}
		else {
			mWidth = LoadSurface->w;
			mHeight = LoadSurface->h;
		}
		SDL_FreeSurface(LoadSurface);
	}
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
void LTexture::setFont(TTF_Font *font)
{
	mFont = font;
}
bool LTexture::loadFromRenderedText(string path, SDL_Color textcolor)
{
	free();
	SDL_Surface *loadSurface = TTF_RenderText_Solid(mFont, path.c_str(), textcolor);
	if (loadSurface == NULL)
	{
		cout << "Failed to loadSurface is null" << TTF_GetError() << endl;
		return false;
	}
	else
	{
		mTexture = SDL_CreateTextureFromSurface(mRenderer, loadSurface);
		if (mTexture == NULL)
		{
			cout << "Unable to create texture from rendered text! Error:" << SDL_GetError() << endl;
			return false;
		}
		else
		{
			mWidth = loadSurface->w;
			mHeight = loadSurface->h;
		}
		SDL_FreeSurface(loadSurface);
		loadSurface = NULL;
	}

	return mTexture != NULL;
}
//TTF_Font *LTexture::mFont = NULL;
#endif // _SDL_TTF_H

void LTexture::setAlpha(Uint8 alpha) 
{
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::setBlendMode(SDL_BlendMode blending) 
{
	SDL_SetTextureBlendMode(mTexture,blending);
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 bule)
{
	SDL_SetTextureColorMod(mTexture,red, green, bule);
}

void LTexture::Render(int x, int y, SDL_Rect *clip,
					double angle,SDL_Point *center,
					SDL_RendererFlip flip)
{
	SDL_Rect RectQuad = {x,y,mWidth,mHeight};

	if (clip != NULL)
	{
		RectQuad.w = clip->w;
		RectQuad.h = clip->h;
	}
	SDL_RenderCopyEx(mRenderer, mTexture, clip, &RectQuad,angle,center,flip);
}

void LTexture::free()
{
	if (mTexture != NULL) 
	{
		SDL_DestroyTexture(mTexture);
		//TTF_CloseFont(mFont);

		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
	
}


SDL_Renderer *LTexture::mRenderer = NULL;


//Button Class

LButton::LButton() 
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}
LButton::~LButton()
{

}

void LButton::setLTexture(LTexture ButtonSpriteSheetTexture)
{
	mButtonSpriteSheetTexture = ButtonSpriteSheetTexture;
}

void LButton::setButtonSize(SDL_Rect *SpriteClips)
{
	mSpriteClips = SpriteClips;
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event *Events)
{
	if (Events->type == SDL_MOUSEMOTION || Events->type == SDL_MOUSEBUTTONDOWN || Events->type == SDL_MOUSEBUTTONUP)
	{
		//Get Mouse postion
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button 
		bool inside = true;

		
		if (x < mPosition.x)						//Mouse is left of the button
		{
			inside = false;
		}
		else
		if (x > mPosition.x + BUTTON_WIDTH)		//Mouse is right of the button
		{
			inside = false;
		}
		else
		if (y < mPosition.y)						//Mouse above the button	
		{
			inside = false;
		}
		else
		if (y > mPosition.y + BUTTON_HEIGHT)		//Mouse below the button
		{
			inside = false;
		}

		if (!inside)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		else
		{
			switch (Events->type)
			{
			case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			default:
				break;
			}
		}
	}
}

void LButton::render()
{
	//Show current button sprite
	mButtonSpriteSheetTexture.Render(mPosition.x, mPosition.y, &mSpriteClips[mCurrentSprite]);
}