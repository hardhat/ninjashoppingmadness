//OSlib header file
#include <stdio.h>
#include <stdlib.h>
#ifdef _PSP
#include <oslib/oslib.h>
#else
#include <SDL/SDL.h>
#ifdef __APPLE__
#include <SDL_image/SDL_image.h>
#include <SDL_mixer/SDL_mixer.h>
#else
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#endif
#endif
#include "main.h"
#include "hud.h"
#include "map.h"
#include "sprite.h"
#include "game.h"

#ifdef _PSP
//Necessary to create eboot
PSP_MODULE_INFO("OSL Jump", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-2048);
#else
SDL_Surface *screen;
Mix_Chunk *sfx[8];
Mix_Music *song;
#endif
const char *sfxpath[]={
	"data/start.wav",
	"data/jump.wav",
	"data/match.wav",
};
enum GameMode gameMode;
int resetTimer;
Game game;


void playSound( SfxType id)
{
	if(id>=0 && id<3 && sfx[id]) {
		Mix_PlayChannel(-1,sfx[id],0);
	}
}


#ifndef _PSP
//#define main SDL_main
#endif
int main(int argc, char **argv)
{
#ifdef _PSP
	oslInit(0);	//Initialization of the Oslib library
	oslInitGfx(OSL_PF_8888, 1);	//Initialization of the graphics mode
#else
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK|SDL_INIT_AUDIO) < 0) exit(1);
	atexit(SDL_Quit);
	//screen=SDL_SetVideoMode(480,272,16,SDL_HWSURFACE|SDL_DOUBLEBUF);
	//screen=SDL_SetVideoMode(640,384 /*480*/,32,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
	screen=SDL_SetVideoMode(1920,1080 /*480*/,32,SDL_HWSURFACE|SDL_DOUBLEBUF /* |SDL_FULLSCREEN */ );
	if(!screen) exit(2);
	SDL_WM_SetCaption( "Ninja Shopping Madness", NULL );
	
	SDL_Joystick *joy=0;
    if(SDL_NumJoysticks()>0)
        joy=SDL_JoystickOpen(0);
    if(SDL_NumJoysticks()>1)
        joy=SDL_JoystickOpen(1);
    printf("Joysticks: %d\n",SDL_NumJoysticks());
    if(Mix_OpenAudio(44100,AUDIO_S16SYS,2,2048)<0) {
    	printf("Warning: couldn't set audio to 44100 Hz\n");
    }
    song=Mix_LoadMUS("data/Seaside.mp3");
    for(int i=0; i<MAXSFX; i++) {
    	sfx[i]=Mix_LoadWAV(sfxpath[i]);
    }
#endif
	
	//loads our images into memory
	Hud hud;
	Map map;
	map.load("data/level.txt");
	Sprite hero("data/Rainbow.png",64,64);
	Sprite baddie("data/White.png",64,64);
	Sprite target("data/girl.png",64,64);
	Mix_PlayMusic(song,-1);
	game.newGame(map,hero,baddie,target);
	printf("New game\n");
	//main while loop
#ifdef _PSP
	while (!osl_quit) {
		oslReadKeys();
		if(osl_keys->released.left) hero.moveLeft(false);
		if(osl_keys->released.right) hero.moveRight(false);
		if(osl_keys->released.cross) hero.jump(false);
		if(osl_keys->pressed.left) hero.moveLeft(true);
		if(osl_keys->pressed.right) hero.moveRight(true);
		if(osl_keys->pressed.cross) hero.jump(true);
#else
	bool done=false;
	while( !done) {
		SDL_Event event;
		printf("Event\n");
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_MOUSEBUTTONDOWN:
				if(gameMode==MODE_WINNER) break;
				if(event.button.x<screen->w/4) {
                    hero.moveLeft(resetTimer==0);
                    hud.leftActive=resetTimer==0;
				} else if(event.button.x>screen->w*3/4) {
                    hero.moveRight(resetTimer==0);
                    hud.rightActive=resetTimer==0;
				} else if(event.button.y>screen->h*3/4) {
                    hero.jump(resetTimer==0);
                    hud.jumpActive=resetTimer==0;
                }
				break;
			case SDL_MOUSEBUTTONUP:
				if(gameMode==MODE_TITLE) gameMode=MODE_MENU;
				else if(gameMode==MODE_MENU) gameMode=MODE_GAME;
				else if(gameMode==MODE_WINNER && resetTimer==0) {
                    gameMode=MODE_TITLE;
                    game.newGame(map,hero,baddie,target);
                }
				if(event.button.x<screen->w/4) {
                    hero.moveLeft(false);
                    hud.leftActive=false;
				} else if(event.button.x>screen->w*3/4) {
                    hero.moveRight(false);
                    hud.rightActive=false;
				} else if(event.button.y>screen->h*3/4) {
                    hero.jump(false);
                    hud.jumpActive=false;
                }
				break;
			case SDL_JOYBUTTONDOWN:
				switch(event.jbutton.button) {
				case 3: hero.moveLeft(resetTimer==0); break;
				case 1: hero.moveRight(resetTimer==0); break;
				case 2: hero.jump(resetTimer==0); break;
				}
				 break;
			case SDL_JOYBUTTONUP:
				if(gameMode==MODE_TITLE) gameMode=MODE_MENU;
				else if(gameMode==MODE_MENU) gameMode=MODE_GAME;
				else if(gameMode==MODE_WINNER && resetTimer==0) gameMode=MODE_MENU;
				switch(event.jbutton.button) {
				case 3: hero.moveLeft(false); break;
				case 1: hero.moveRight(false); break;
				case 2: hero.jump(false); break;
				}
				break;

			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
				case SDLK_LEFT: hero.moveLeft(resetTimer==0); break;
				case SDLK_RIGHT: hero.moveRight(resetTimer==0); break;
				case SDLK_UP: hero.jump(resetTimer==0); break;
				case 'a': baddie.moveLeft(resetTimer==0); break;
				case 'd': baddie.moveRight(resetTimer==0); break;
				case 'w': baddie.jump(resetTimer==0); break;
				}
				break;
			case SDL_KEYUP:
				switch(event.key.keysym.sym) {
				case SDLK_LEFT: hero.moveLeft(false); break;
				case SDLK_RIGHT: hero.moveRight(false); break;
				case SDLK_UP: hero.jump(false); break;
				case 'a': baddie.moveLeft(false); break;
				case 'd': baddie.moveRight(false); break;
				case 'w': baddie.jump(false); break;
				case SDLK_RETURN: game.newGame(map,hero,baddie,target); break;
				case SDLK_ESCAPE:
					if(gameMode==MODE_MENU) {
						printf("Game mode: mode_menu, done\n");
						done=true;
					} else {
						printf("Game mode: %d\n", (int)gameMode);
						game.newGame(map,hero,baddie,target);
						gameMode=MODE_TITLE;
					}
					break;
				}
				if(gameMode==MODE_TITLE) gameMode=MODE_MENU;
				else if(gameMode==MODE_MENU) gameMode=MODE_GAME;
				break;
			case SDL_QUIT:
				done=1;
				break;
			}
		}
#endif
    	game.update(map,hero,baddie,target,hud);
        if(gameMode==MODE_GAME && resetTimer==0) {
    		baddie.ai(&map,&target);
        }
    
    	game.update(map,hero,baddie,target,hud);
    	if(gameMode==MODE_GAME && resetTimer==0) {
    		baddie.ai(&map,&target);
        }

		printf("Entry + 1 Beer\n");
		game.draw(map,hero,baddie,target,hud);
	}
	
	printf("Exiting...\n");
	//Terminate the program
#ifdef _PSP
	oslEndGfx();
	oslQuit();
#endif
	return 0;
}


#ifdef __QNX__
void SDL_free(void *mem)
{
	free(mem);
}

void *SDL_malloc(size_t size)
{
	return malloc(size);
}

void *SDL_realloc(void *mem,size_t size)
{
	return realloc(mem,size);
}

void *SDL_calloc(size_t count,size_t size)
{
	return calloc(count,size);
}

extern "C" {

char *SDL_strdup(const char *mem)
{
	return strdup(mem);
}

int SDL_vsnprintf(char * str, size_t size, const char * format, va_list ap)
{
	return vsnprintf(str,size,format,ap);
}

}

char* SDL_getenv(const char *name)
{
	return getenv(name);
}

#endif
