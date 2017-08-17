#include <SDL.h>
#include "rayt.h"
#include "input.h"
#include "player.h"
#include "video.h"
#include <vector>
#include "map.h"
extern Player play;
void HandleEvent(SDL_Event &e){
	if(e.type==SDL_QUIT)Exit(1);
	if(e.type==SDL_KEYDOWN)KeybDown(e);	
	if(e.type==SDL_KEYUP)KeybUp(e);	
	if(e.type==SDL_ACTIVEEVENT){
		if(e.active.state==SDL_APPMOUSEFOCUS )return;
		if(e.active.gain==1){
			WakeUp();
		}else{
			Sleep();
		}
	}
}

void KeybDown(SDL_Event &e){
	switch(e.key.keysym.sym){
		case SDLK_LEFT:
			play.KeyLeft(true);
		break;
		case SDLK_RIGHT:
			play.KeyRight(true);
		break;
		case SDLK_UP:
			play.KeyUp(true);
		break;
		case SDLK_DOWN:
			play.KeyDown(true);
		break;
		case SDLK_ESCAPE:
			Exit(2);
		break;
		case SDLK_LSHIFT:
		case SDLK_RSHIFT:
			play.KeyRun(true);
		break;
		case SDLK_r:fov+=5;break;
		case SDLK_f:fov-=5;break;
		case SDLK_t:maxclip+=10;break;
		case SDLK_g:maxclip-=10;break;
#define SHIFTAMT 8
		case SDLK_q:ShiftR(SHIFTAMT);break;
		case SDLK_w:ShiftG(SHIFTAMT);break;
		case SDLK_e:ShiftB(SHIFTAMT);break;
		case SDLK_a:ShiftR(-SHIFTAMT);break;
		case SDLK_s:ShiftG(-SHIFTAMT);break;
		case SDLK_d:ShiftB(-SHIFTAMT);break;
		case SDLK_z:
			ShiftR(SHIFTAMT);
			ShiftG(SHIFTAMT);
			ShiftB(SHIFTAMT);
		break;
		case SDLK_x:
			ShiftR(-SHIFTAMT);
			ShiftG(-SHIFTAMT);
			ShiftB(-SHIFTAMT);
		break;

	}
}

void KeybUp(SDL_Event &e){
	switch(e.key.keysym.sym){
		case SDLK_LEFT:
			play.KeyLeft(false);
		break;
		case SDLK_RIGHT:
			play.KeyRight(false);
		break;
		case SDLK_UP:
			play.KeyUp(false);
		break;
		case SDLK_DOWN:
			play.KeyDown(false);
		break;
		case SDLK_LSHIFT:
		case SDLK_RSHIFT:
			play.KeyRun(false);
		break;
		case SDLK_ESCAPE:
			Exit(2);
		break;
		case SDLK_1:
			SetRes(320);
		break;
		case SDLK_2:
			SetRes(160);
		break;
		case SDLK_3:
			SetRes(80);
		break;
		case SDLK_4:
			SetRes(40);
		break;
		case SDLK_5:
			SetRes(20);
		break;
		case SDLK_6:
			SetRes(10);
		break;
		case SDLK_7:
			SetRes(5);
		break;
		case SDLK_8:
			SetRes(2);
		break;
		case SDLK_9:
			SetRes(100);
		break;
		case SDLK_0:
			SetRes(64);
		break;
		case SDLK_F2:
			DumpToFile("output.xml");
		break;
		case SDLK_F3:
			LoadMapFromFile("main.xml");
		break;
		case SDLK_HOME:
			play.GoHome();
			break;
	}
}