#include <SDL.h>
#include <SDL_opengl.h>
#include "rayt.h"
#include "player.h"
#include "video.h"
#include <vector>
#include "map.h"
#include <math.h>

const float movscale=0.2f;
const float movrunscale=0.6f;
const float rotscale=0.2f;
const float rotrunscale=0.6f;
const float collidescale=10.00f;
const float vectlength=250;
void Player::Move(){
	float radrot=(rot/180.0f)*PI;
	float rscale=(key_run)?(rotrunscale):(rotscale);

	if(key_left)rot-=rscale;
	if(key_right)rot+=rscale;

	ForwardSeg.start.x=x;
	ForwardSeg.start.y=y;
	if(key_up){
		ForwardSeg.end.x=x+(sinf(radrot)*collidescale);
		ForwardSeg.end.y=y-(cosf(radrot)*collidescale);

		if(TestAgainstMap(ForwardSeg)==-1){
			float movspeed=movscale;
			if(key_run){
				movspeed=movrunscale;
			}
			x+=(sinf(radrot)*movspeed);
			y-=(cosf(radrot)*movspeed);
		}
	}
	if(key_down){
		float invradrot=((rot+180)/180.0f)*PI;
		ForwardSeg.end.x=x+(sinf(invradrot)*collidescale);
		ForwardSeg.end.y=y-(cosf(invradrot)*collidescale);

		if(TestAgainstMap(ForwardSeg)==-1){
			float movspeed=movscale;
			if(key_run){
				movspeed=movrunscale;
			}
			x+=(sinf(invradrot)*movspeed);
			y-=(cosf(invradrot)*movspeed);
		}
	}
	float radrotl=((rot-(fov/2.0f))/180.0f)*PI;
	float radrotr=((rot+(fov/2.0f))/180.0f)*PI;
	
	
	LeftFov.start=ForwardSeg.start;
	RightFov.start=ForwardSeg.start ;
	
	LeftFov.end.x=x+(sinf(radrotl)*vectlength);
	LeftFov.end.y=y-(cosf(radrotl)*vectlength);
	RightFov.end.x=x+(sinf(radrotr)*vectlength);
	RightFov.end.y=y-(cosf(radrotr)*vectlength);

}



Player::Player(): x(0),y(0),rot(0){
	LeftFov.col.r=10;
	LeftFov.col.g=157;
	LeftFov.col.b=245;
	RightFov=LeftFov;
}
Player::~Player(){
	//Do nothing
}
void Player::Draw(){
	glPushMatrix();
		glTranslatef(x,y,0);
		glRotatef(rot,0,0,1);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0,-10);
			glVertex2i(-5,10);
			glVertex2i(5,10);
		glEnd();
	glPopMatrix();
}

void Player::ZDraw(float nx,float ny){
	glPushMatrix();
		glTranslatef(nx,ny,0);
		glRotatef(rot,0,0,1);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0,-10);
			glVertex2i(-5,10);
			glVertex2i(5,10);
		glEnd();
	glPopMatrix();
}
