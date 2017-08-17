#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "rayt.h"
#include "input.h"
#include "video.h"
#include "player.h"
#include "map.h"

Player play;
int main(int argc,char* argv[]){
	if(SDL_Init(SDL_INIT_VIDEO)==-1){
		printf("Error initing SDL: %S\n",SDL_GetError());
		return -1;
	}
	SDL_WM_SetCaption("Rayt","Rayt");
	SDL_Surface *screen=SDL_SetVideoMode(608,280,0,SDL_OPENGL|SDL_ANYFORMAT);
	if(!screen){
		printf("Error setting video mode: %s\n",SDL_GetError());
		Exit(-1);
	}
	Setup(608,280);
	//play.x=120;play.y=120;
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
	MapInit();
	SDL_Event ev;
	for(;;){
		while(SDL_PollEvent(&ev)){
			HandleEvent(ev);
		}
		play.Move();
		Render();
	}
	return 42;
}

void Exit(int code){

	SDL_Quit();
	exit(code);
}
