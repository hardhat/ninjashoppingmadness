#include <SDL/SDL.h>
#include <SDL_image/SDL_image.h>
#include "hud.h"
#include "falling.h"

extern SDL_Surface *screen;

Hud::Hud()
{
	numbers=IMG_Load("data/numbers.png");
	winner=IMG_Load("data/winner.png");
	loser=IMG_Load("data/loser.png");
	icon[0]=IMG_Load("data/blue_ninjaicon.png");
	icon[1]=IMG_Load("data/teethicon.png");
	leftScore=0;
	rightScore=0;
	who=0;
	whoX=0;
	whoY=0;
	leftX=0;
	leftY=0;
	rightX=screen->w-64;
	rightY=0;
	leftActive=false;
	rightActive=false;
	jumpActive=false;
}

void Hud::update(Falling *hero, Falling *baddie)
{
	leftScore=hero->score;
	rightScore=baddie->score;
	
	leftX-=5;
	if(leftX<0) leftX=0;
	leftY-=5;
	if(leftY<0) leftY=0;
	rightX+=5;
	if(rightX>screen->w-64) rightX=screen->w-64;
	rightY-=5;
	if(rightY<0) rightY=0;
	
	if(who==hero) {
		leftX=whoX;
		leftY=whoY;
		who=0;	
	}
	if(who==baddie) {
		rightX=whoX;
		rightY=whoY;
		who=0;
	}
}

void Hud::animateScore(int viewx,int viewy,Falling *who)
{
	this->who=who;
	this->whoX=who->x-viewx;
	this->whoY=who->y-viewy;	
}

void Hud::drawDigit(int x,int y,int i)
{
	SDL_Rect src={static_cast<Sint16>(i*32),0,32,32};
	SDL_Rect dest={static_cast<Sint16>(x),static_cast<Sint16>(y),32,32};
	SDL_BlitSurface(numbers,&src,screen,&dest);
}

void Hud::drawArrow(int x,int y,int i)
{
	drawDigit(x,y,i+10);
}

void Hud::drawIcon(int x,int y,SDL_Surface *image)
{
	SDL_Rect dest={static_cast<Sint16>(x),static_cast<Sint16>(y),0,0};
	SDL_BlitSurface(image,NULL,screen,&dest);
}

void Hud::drawDigit(SDL_Surface *icon,int x,int y,int i)
{
	drawIcon(x,y,icon);
	drawDigit(x+32,y,i);
}


void Hud::draw()
{
	if(leftScore>=9) {
		drawIcon(screen->w/2-winner->w/2,100,winner);
	}
	if(rightScore>=9) {
		drawIcon(screen->w/2-loser->w/2,100,loser);
	}
	drawDigit(icon[0],(int)leftX,(int)leftY,leftScore);
	drawDigit(icon[1],(int)rightX,(int)rightY,rightScore);
	
	if(leftActive) drawArrow(10,screen->h/2,3);
	drawArrow(10,screen->h/2,0);
	if(rightActive) drawArrow(screen->w-32-10,screen->h/2,3);
	drawArrow(screen->w-32-10,screen->h/2,1);
	if(jumpActive) drawArrow(screen->w/2-16,screen->h-10-32,3);
	drawArrow(screen->w/2-16,screen->h-10-32,2);
}
