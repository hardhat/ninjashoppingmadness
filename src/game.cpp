#include <SDL/SDL.h>
#ifdef __APPLE__
#include <SDL_mixer/SDL_mixer.h>
#else
#include <SDL/SDL_mixer.h>
#endif

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

void Game::mapReset(Map &map)
{
//	hero.reset(64,128);
//	baddie.reset(64,128);
//	i=(rand()%(map.getTilesAcross()-3))+2;
//	j=(rand()%(map.getTilesDown()-2))+1;
//	target.reset(32*i,32*j);
    int x,j;
    for( SpriteList::iterator p=_playerList.begin(); p!=_playerList.end(); p++) {
        x=(rand()%(map.getTilesAcross()-3))+2;
        j=(rand()%(map.getTilesDown()-2))+1;
        Sprite *player=*p;
        player->reset(32*x, 32*j);
    }
    for( SpriteList::iterator p=_itemList.begin(); p!=_itemList.end(); p++) {
        x=(rand()%(map.getTilesAcross()-3))+2;
        j=(rand()%(map.getTilesDown()-2))+1;
        Sprite *item=*p;
        item->reset(32*x, 32*j);
    }
//	map.calculateGradient(&target);
//	resetTimer=3000;
//	Mix_PauseMusic();
//	map.updateView(&target);
}

void Game::newGame(Map &map)
{
    //for (int i = 0; i < _playerList.size(); i++) {
    for( SpriteList::iterator p=_playerList.begin(); p!=_playerList.end(); p++) {
        Sprite *player=*p;
        player->score = 0;
    }
    for( SpriteList::iterator p=_itemList.begin(); p!=_itemList.end(); p++) {
        Sprite *item=*p;
        item->score = 0;
    }
	mapReset(map);
	playSound(S_START);
}

void Game::update(Map &map,Hud &hud)
{
	map.updatePhysics();
    for( SpriteList::iterator p=_playerList.begin(); p!=_playerList.end(); p++) {
        Sprite *player=*p;
        player->updatePhysics(&map);
    }
    for( SpriteList::iterator p=_itemList.begin(); p!=_itemList.end(); p++) {
        Sprite *item=*p;
        item->updatePhysics(&map);
    }
	//baddie.x=200;
//	if(resetTimer>0) {
//		resetTimer-=16;
//		if(resetTimer<=0) {
//			resetTimer=0;
//			Mix_ResumeMusic();
//		}
//	} else {
//		map.updateView(&hero); //, &baddie);
//	}
//	hud.update(&hero, &baddie);
//	if( hitTarget(&hero,&target,map,hud) || hitTarget(&baddie,&target,map,hud)) {
//		mapReset(map);
//		playSound(S_MATCH);
//		if(hud.leftScore>8 || hud.rightScore>8) gameMode=MODE_WINNER;
//	}
}

SDL_Surface *titleImage=0;
SDL_Surface *menuImage=0;
void Game::draw(Map &map,Hud &hud)
{
#ifdef _PSP
	oslStartDrawing();		//To be able to draw on the screen
#else
	static SDL_Surface *bgImage=0;
	//if(!bgImage) bgImage=IMG_Load("data/title.png");
	//if(bgImage) SDL_BlitSurface(bgImage,0,screen,0);
	//else 
    SDL_FillRect(screen,0,SDL_MapRGB(screen->format,189, 237, 255));
#endif
	
	map.draw();		//Draw the images to the screen
    for( SpriteList::iterator p=_playerList.begin(); p!=_playerList.end(); p++) {
        Sprite *player=*p;
        player->draw(map.viewx,map.viewy);
    }
    for( SpriteList::iterator p=_itemList.begin(); p!=_itemList.end(); p++) {
        Sprite *item=*p;
        item->draw(map.viewx,map.viewy);
    }
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

void Game::addCharSprite(Sprite* spriteToAdd){
    _playerList.push_back(spriteToAdd);
    printf("Success\n");
}

void Game::addItemSprite(Sprite *spriteToAdd){
    _itemList.push_back(spriteToAdd);
}

