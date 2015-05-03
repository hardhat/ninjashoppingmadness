#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "main.h"
#include "game.h"
#include "map.h"
#include "hud.h"
#include "sprite.h"
extern int resetTimer;
extern SDL_Surface *screen;

Game::Game()
{
    
}

bool Game::hitTarget(Sprite *hero,Sprite *target,Map &map,Hud &hud)
{
	float deltax,deltay;
	float dist;
	deltax=target->x-hero->x;
	deltay=target->y-hero->y;
	dist=deltax*deltax+deltay*deltay;
	if(dist<32*32) {
		hero->score++;
		hud.animateScore(map.viewx,map.viewy,hero);
		return true;
	}
	return false;
}

void Game::mapReset(Map &map,Sprite &hero,Sprite &baddie,Sprite &target)
{
	hero.reset(64,128);
	baddie.reset(64,128);
	int i,j;
	i=(rand()%(map.getTilesAcross()-3))+2;
	j=(rand()%(map.getTilesDown()-2))+1;
	target.reset(32*i,32*j);
	map.calculateGradient(&target);
	resetTimer=3000;
	Mix_PauseMusic();
	map.updateView(&target);
}

void Game::newGame(Map &map,Sprite &hero,Sprite &baddie,Sprite &target)
{
	hero.score=0;
	baddie.score=0;
	target.score=0;
	mapReset(map,hero,baddie,target);
	playSound(S_START);
}

void Game::update(Map &map,Sprite &hero,Sprite &baddie,Sprite &target,Hud &hud)
{
	map.updatePhysics();
	hero.updatePhysics(&map);
	baddie.updatePhysics(&map);
	target.updatePhysics(&map);
	//baddie.x=200;
	if(resetTimer>0) {
		resetTimer-=16;
		if(resetTimer<=0) {
			resetTimer=0;
			Mix_ResumeMusic();
		}
	} else {
		map.updateView(&hero); //, &baddie);
	}
	hud.update(&hero, &baddie);
	if( hitTarget(&hero,&target,map,hud) || hitTarget(&baddie,&target,map,hud)) {
		mapReset(map,hero,baddie,target);
		playSound(S_MATCH);
		if(hud.leftScore>8 || hud.rightScore>8) gameMode=MODE_WINNER;
	}
}

SDL_Surface *titleImage=0;
SDL_Surface *menuImage=0;
void Game::draw(Map &map,Sprite &hero,Sprite &baddie,Sprite &target,Hud &hud)
{
#ifdef _PSP
	oslStartDrawing();		//To be able to draw on the screen
#else
	static SDL_Surface *bgImage=0;
	//if(!bgImage) bgImage=IMG_Load("data/title.png");
	//if(bgImage) SDL_BlitSurface(bgImage,0,screen,0);
	//else 
    SDL_FillRect(screen,0,SDL_MapRGB(screen->format,240,240,128));
#endif
	
	map.draw();		//Draw the images to the screen
	hero.draw(map.viewx,map.viewy);
	baddie.draw(map.viewx,map.viewy);
	target.draw(map.viewx,map.viewy);
	hud.draw();
	if( gameMode==MODE_TITLE) {
		if(!titleImage) titleImage=IMG_Load("data/title.png");
		if(titleImage) SDL_BlitSurface( titleImage, NULL, screen, NULL);
	} else {
		if(titleImage) SDL_FreeSurface( titleImage);
		titleImage=0;
	}
	if( gameMode==MODE_MENU) {
		if(!menuImage) menuImage=IMG_Load("data/menu.png");
		if(menuImage) SDL_BlitSurface( menuImage, NULL, screen, NULL);
	} else {
		if(menuImage) SDL_FreeSurface( menuImage);
		menuImage=0;
	}

#ifdef _PSP
	oslEndDrawing();		//Ends drawing mode

	oslEndFrame();
	oslSyncFrame();		//Synchronizes the screen
#else
	SDL_Flip(screen);
	static long before;
	long now=SDL_GetTicks();
	long delay=before+32-now;
	if(delay>0 && delay<60) SDL_Delay(delay);
	before=now;
#endif
}

void Game::handleUp(int key)
{
    
}

void Game::handleDown(int key)
{
    
}

