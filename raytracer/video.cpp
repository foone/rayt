#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include "rayt.h"
#include "video.h"
#include "player.h"
#include "map.h"
#include <math.h>
extern Player play;

SDL_Rect raybox={16,16,320,240},topbox={320+32,16,240,240};
float linedist[320];
Color linecol[320];
int linecount=320;
int segmentat[320];
int selected_seg=9;
bool sleeping=false;
int scr_width,scr_height;
void Sleep(){sleeping=true;}
void WakeUp(){sleeping=false;}

void SetRes(int lines){linecount=lines;}
void Render(){
	if(sleeping)return;
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	//for(int i=0;i<320;i++)linedist[i]=8192;
	RenderUI();
	
	DrawTopMap();
	Raytrace();
	DrawLines();
	
	SDL_GL_SwapBuffers();
}

void Setup(int width, int height){
	glDisable(GL_DITHER);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0,width-1,height-1,0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(1.0f,1.0f,1.0f,0.1f);
	scr_width=width;scr_height=height;
	ResetViewport();
	glScissor(raybox.x+1,scr_height-(raybox.y+raybox.h),320,240);

	
	
}
void ResetViewport(){
	glViewport(0,0,scr_width,scr_height);
}
void RenderUI(){
	glColor3ub(0,0,0);
	glBegin(GL_LINE_LOOP);
		glVertex2i(raybox.x,raybox.y);
		glVertex2i(raybox.x,raybox.y+raybox.h);
		glVertex2i(raybox.x+raybox.w+1,raybox.y+raybox.h);
		glVertex2i(raybox.x+raybox.w+1,raybox.y);
	glEnd();
	/*
	glBegin(GL_LINE_LOOP);
		glVertex2i(topbox.x,topbox.y);
		glVertex2i(topbox.x,topbox.y+topbox.h);
		glVertex2i(topbox.x+topbox.w,topbox.y+topbox.h);
		glVertex2i(topbox.x+topbox.w,topbox.y);
	glEnd();
	*/
	glColor3ub(192,192,192);
	glBegin(GL_QUADS);
		//Top down view
		glVertex2i(topbox.x+1,topbox.y);
		glVertex2i(topbox.x+1,topbox.y+topbox.h-1);
		glVertex2i(topbox.x+topbox.w,topbox.y+topbox.h-1);
		glVertex2i(topbox.x+topbox.w,topbox.y);
		//3d floor
		glVertex2i(raybox.x+1,raybox.y+raybox.h/2);
		glVertex2i(raybox.x+raybox.w,raybox.y+raybox.h/2);
		glVertex2i(raybox.x+raybox.w,raybox.y+raybox.h-1);
		glVertex2i(raybox.x+1,raybox.y+raybox.h-1);

		glColor3ub(120,120,255);
		// 3d sky
		glVertex2i(raybox.x+1,raybox.y+1);
		glVertex2i(raybox.x+raybox.w,raybox.y+1);
		glVertex2i(raybox.x+raybox.w,raybox.y+raybox.h/2);
		glVertex2i(raybox.x+1,raybox.y+raybox.h/2);
	glEnd();
	glColor3ub(120,120,255);
	glBegin(GL_TRIANGLES);
		
		glVertex2i((raybox.x+raybox.w/2),raybox.y-2);
		glVertex2i((raybox.x+raybox.w/2)+10,raybox.y-10);
		glVertex2i((raybox.x+raybox.w/2)-10,raybox.y-10);


	glEnd();
}


void DrawTopMap(){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-120,120,120,-120);
		glViewport(topbox.x,(scr_height-(topbox.y+topbox.h)),topbox.w,topbox.h);
	glMatrixMode(GL_MODELVIEW);
	glColor3ub(120,120,255);
	play.Draw();	
	DrawPlayerOut();
	glBegin(GL_LINES);
	std::vector<LineSeg>::iterator i,e=map.end();
		for(i=map.begin();i!=e;i++){
			LineSeg &l=*i;
			DrawSeg(l);
			
		}
	
	if(IsIn(play.x,play.y)){
		DrawSeg(play.LeftFov);
		DrawSeg(play.RightFov);
	}
	
	glEnd();
	
	glColor3ub(0,0,0);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	//printf("%i,%i\n",p.x,p.y);
	glMatrixMode(GL_MODELVIEW);

	ResetViewport();
}

void DrawSeg(const LineSeg &s){
	glColor3ub(s.col.r,s.col.g,s.col.b);
	glVertex2i(s.start.x,s.start.y);
	glVertex2i(s.end.x,s.end.y);
}
void DrawSegRed(const LineSeg &s){
	glColor3ub(255,0,0);
	glVertex2i(s.start.x,s.start.y);
	glVertex2i(s.end.x,s.end.y);
}

void DrawLines(){
	glEnable(GL_SCISSOR_TEST);
	float mult=160/tan((fov/180)*PI);
	float h=0; // Half Height
	float lh=0;
	float rh=0,rhd;
	float c;
	int a,a2;
	float r,g,b;
	for(int i=0;i<320;i++){
		a=i-(i%(320/linecount));
		if(linedist[a]<maxclip){
			h=(32/linedist[a])*mult;
			if(linecount!=320 && i!=a){
				a2=a-(320/linecount);
				if(a2<0)b=0;
				lh=(32/linedist[a2])*mult;
				a2=a+(320/linecount);
				rh=(32/linedist[a2])*mult;
				c=(i%(320/linecount))/(320/linecount);
				rhd=rh-h;
				h=h+(rhd*c);
				
			}
			r=linecol[a].r/255.0f;
			g=linecol[a].g/255.0f;
			b=linecol[a].b/255.0f;
			float fade=1-(linedist[a]/foglimit);
			if(fade<0)fade=0;
			r*=fade;
			g*=fade;
			b*=fade;
			glBegin(GL_LINES);
			glColor3f(r,g,b);
			glVertex2i(raybox.x+i+1,(raybox.y+(raybox.h/2))-h);
			glColor3f(r/2.0f,g/2.0f,b/2.0f);
			glVertex2i(raybox.x+i+1,(raybox.y+(raybox.h/2))+h);
			glEnd();

			glColor3ub(0,0,0);
			glBegin(GL_POINTS);
			glVertex2i(raybox.x+i+1,(raybox.y+(raybox.h/2))-h);
			glVertex2i(raybox.x+i+1,(raybox.y+(raybox.h/2))+h);
			glEnd();

		}
	}
	glDisable(GL_SCISSOR_TEST);	
}

int GetSegmentAtPointer(){
	// Who needs a mouse?
	return segmentat[160];
}
void ShiftR(int amt){
	int ne=GetSegmentAtPointer();
	if(ne<0)return;
	if(ne>=map.size())return;
	int color=map[ne].col.r;
	color+=amt;
	if(color<0)color=0;
	if(color>255)color=255;
	map[ne].col.r=color;
}
void ShiftG(int amt){
	int ne=GetSegmentAtPointer();
	if(ne<0)return;
	if(ne>=map.size())return;
		int color=map[ne].col.g;
	color+=amt;
	if(color<0)color=0;
	if(color>255)color=255;
	map[ne].col.g=color;
}
void ShiftB(int amt){
	int ne=GetSegmentAtPointer();
	if(ne<0)return;
	if(ne>=map.size())return;
	int color=map[ne].col.b;
	color+=amt;
	if(color<0)color=0;
	if(color>255)color=255;
	map[ne].col.b=color;
}

void SetSelected(int sel){selected_seg=sel;}

void DrawPlayerOut(){
	if(IsIn(play.x,play.y))return;
	glColor3ub(255,128,64);
	
	Point p=GetOutsidePlayerClosestWallPoint();
	play.ZDraw(p.x,p.y);

}