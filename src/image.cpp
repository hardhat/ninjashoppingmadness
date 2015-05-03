#include <stdio.h>
#include "image.h"
#include "map.h"
#include "main.h"
#ifdef __APPLE__
#include <SDL_image/SDL_image.h>
#include <SDL_mixer/SDL_mixer.h>
#else
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#endif

#ifndef _PSP
extern SDL_Surface *screen;
#else
extern struct Screen {
    int w,h;
} *screen;
#endif

// OSL_Image *image;
// int cellw;
// int cellh;
// int frame,frameTimer;
// int direction;

Image::Image(const char *imageFile)
{
    cellw=64;
    cellh=64;
    width=64*3/4;
    height=64;
#ifdef _PSP
    image=oslLoadImageFile((char *)spriteSheet, OSL_IN_RAM, OSL_PF_5551);
#else
    image=IMG_Load(imageFile);
#endif
    frame=0;
    facing=FACING_RIGHT;
    ax=0;
    vx=0;
//    state=CS_STANDING;
    frameTimer=1;
}

Image::~Image()
{
#ifdef _PSP
    if(image) oslDeleteImage(image);
#else
    if(image) SDL_FreeSurface(image);
#endif
    image=0;
}

void Image::draw(int viewx,int viewy)
{
    int top,left;
    left=(frame%10)*cellw; // convert from tiles across to pixels across.
    top=(frame/10)*cellh;	// convert from tiles down to pixels down.
#ifdef _PSP
    oslSetImageTileSize(image,left,top,cellw,cellh);	// set the sprite sheet up to show that sprite.
    
    image->x=x-width/2-viewx;	// draw it where it goes
    image->y=y-height/2-viewy+4;	// draw it where it goes
    if(facing==FACING_LEFT) oslMirrorImageH(image);	// flip
    oslDrawImage(image);
    if(facing==FACING_LEFT) oslMirrorImageH(image);	// return to normal.
#else
    // doesn't do facing. :(
    SDL_Rect src={static_cast<Sint16>(left),static_cast<Sint16>(top),static_cast<Uint16>(cellw),static_cast<Uint16>(cellh)},dest={static_cast<Sint16>(x-width/2-viewx),static_cast<Sint16>(y-height/4-viewy),static_cast<Uint16>(cellw),static_cast<Uint16>(cellh)};
    SDL_BlitSurface(image,&src,screen,&dest);
#endif
}

void Image::reset(int x,int y)
{
    this->x=x;
    this->y=y;
    vx=0;
    vy=0;
    ax=0;
    ay=0;
    
}
