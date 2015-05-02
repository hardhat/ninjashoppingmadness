#ifndef HUD_H
#define HUD_H
#include <SDL/SDL.h>

class Falling;

class Hud
{
public:
	Hud();
	void update(Falling *hero,Falling *baddie);
	void draw();
	void drawDigit(int x,int y,int i);
	void drawDigit(SDL_Surface *icon,int x,int y,int i);
	void drawArrow(int x,int y,int i);
	void drawIcon(int x,int y, SDL_Surface *surf);
	void animateScore(int viewx,int viewy,Falling *who);

	SDL_Surface *numbers,*winner,*loser,*icon[2];
	int leftScore,rightScore;
	Falling *who;
	float whoX,whoY;
	float leftX,leftY;
	float rightX,rightY;
	bool leftActive,rightActive,jumpActive;
};

#endif
