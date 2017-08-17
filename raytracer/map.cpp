#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <tinyxml.h>
#include "math.h"
#include <vector>
#include "rayt.h"
#include "input.h"
#include "video.h"
#include "player.h"
#include "map.h"
#include <algorithm>

std::vector<LineSeg> map;
extern Player play;
double maxclip=512.0f;
double foglimit=1024.0f;
double fov=60.0f;
void MapInit(){
	map.clear();
	LoadMapFromFile("default.xml");
/*
	CreateShadedBox(-75,-75,10,10,Color(255,255,255),Color(230,230,230));
	
	CreateShadedBox(-75,-35,10,30,Color(199,120,255),Color(189,110,230));
	
	CreateShadedBox(-75,70,20,20,Color(15,5,199),Color(84,75,250));

	CreateShadedBox(60,0,20,5,Color(236,155,57),Color(153,91,15));
	CreateShadedBox(60,0,5,20,Color(39,215,239),Color(12,138,156));


	CreateShadedBox(0,100,5,5,Color(128,128,128),Color(168,168,168));
	*/
}
void CreateBox(int x,int y,int w,int h,Color c){
CreateShadedBox(x,y,w,h,c,c);
}
void CreateShadedBox(int x,int y,int w,int h,Color ns,Color ew){
	LineSeg ls;
	Point p[4];
	int w2=w/2;
	int h2=h/2;
	p[0].x=x-w;
	p[0].y=y-h;
	
	p[1].x=x+w;
	p[1].y=y-h;
	
	p[2].x=x+w;
	p[2].y=y+h;

	p[3].x=x-w;
	p[3].y=y+h;

	ls.start=p[0];
	ls.end=p[1];
	ls.col=ew;
	map.push_back(ls);
	ls.start=p[1];
	ls.end=p[2];
	ls.col=ns;
	map.push_back(ls);
	ls.start=p[2];
	ls.end=p[3];
	ls.col=ew;
	map.push_back(ls);
	ls.start=p[3];
	ls.end=p[0];
	ls.col=ns;
	map.push_back(ls);



}

bool __inline LineSegIntersect(const LineSeg &l1,const LineSeg &l2,Point &p){
	double r,br,tr,s;
	const Point &a=l1.start;
	const Point &b=l1.end;
	const Point &c=l2.start;
	const Point &d=l2.end;
	br=((b.x-a.x)*(d.y-c.y))-((b.y-a.y)*(d.x-c.x));
	if(br==0)return false;
	tr=((a.y-c.y)*(d.x-c.x))-((a.x-c.x)*(d.y-c.y));
	r=tr/br;
	tr=((a.y-c.y)*(b.x-a.x))-((a.x-c.x)*(b.y-a.y));
	s=tr/br;
	if(r<0)return false;
	if(r>1)return false;
	if(s<0)return false;
	if(s>1)return false;
	p.x=(a.x+(r*(b.x-a.x)));
	p.y=(a.y+(r*(b.y-a.y)));
	return true;
}

double GetDist(Point &a,Point &b){
	return sqrt(((a.x-b.x)*(a.x-b.x))+((a.y-b.y)*(a.y-b.y)));
}
const int lines=320;

extern float linedist[320];
extern Color linecol[320];
extern int segmentat[320];
void Raytrace(){
	LineSeg ray;
	double radrot,rot=play.rot;
	double dist;
	Point p,px(play.x,play.y);
	ray.start=px;
	int s=map.size();
	double adj;
	double linescale;
	for (int i=0;i<lines;i++){
		adj=((i-160)/160.0f)*(fov/2.0f);
		linescale=cos((i-160)/320.0f);
		radrot=((rot+adj)/180.0f)*PI;
		ray.end.x=play.x+(sin(radrot)*maxclip);
		ray.end.y=play.x-(cos(radrot)*maxclip);
		linedist[i]=8192.0f;
		segmentat[i]=-1;
		for(int q=0;q<s;q++){
			if(LineSegIntersect(ray,map[q],p)){
				dist=GetDist(p,px)*linescale;
				if(dist<linedist[i]){
					linedist[i]=dist;
					linecol[i]=map[q].col;
					segmentat[i]=q;
				}
			}
		}
		
	}
}
int TestAgainstMap(LineSeg &c){
	int s=map.size();
	Point p;
	for (int i=0;i<s;i++){
		if(LineSegIntersect(c,map[i],p))return i;
	}
	return -1;
}
void DumpToFile(const char *name){
	// Hah, who needs XML library?
	FILE *fp=fopen(name,"w");
	if(!fp)return;
	int i,s=map.size();
	fputs("<?xml version=\"1.0\" standalone=\"yes\" ?>\n",fp);
	fputs("<RaytWorld>\n",fp);
	for(i=0;i<s;i++){
		fputs("\t<LineSegment>\n",fp);
		LineSeg l=map[i];
		fprintf(fp,"\t\t<Point x=\"%0.2f\" y=\"%0.2f\" />\n",l.start.x,l.start.y);
		fprintf(fp,"\t\t<Point x=\"%0.2f\" y=\"%0.2f\" />\n",l.end.x,l.end.y);
		fprintf(fp,"\t\t<Color r=\"%i\" g=\"%i\" b=\"%i\" />\n",l.col.r,l.col.g,l.col.b);
		fputs("\t</LineSegment>\n",fp);
	}
	fputs("</RaytWorld>\n",fp);
	fclose(fp);
}

Point GetOutsidePlayerClosestWallPoint(){
	Point p(0,0);
	LineSeg PlayCast(play.x,play.y,0,0);
	float dist=100;
	LineSeg top(-dist,-dist,dist,-dist);
	LineSeg left(-dist,-dist,-dist,dist);
	LineSeg right(dist,-dist,dist,dist);
	LineSeg bottom(-dist,dist,dist,dist);
	if(LineSegIntersect(PlayCast,top,p))return p;
	if(LineSegIntersect(PlayCast,left,p))return p;
	if(LineSegIntersect(PlayCast,right,p))return p;
	if(LineSegIntersect(PlayCast,bottom,p))return p;
	return Point(0,0);
}

bool IsIn(float x,float y){
	if(x<-128)return false;
	if(y<-128)return false;
	if(x>128)return false;
	if(y>128)return false;
	return true;
}

void LoadMapFromFile(const char *name){
	TiXmlDocument doc;
	if(!doc.LoadFile(name))return;
	TiXmlElement *root=doc.RootElement();
	if(!root)return;
	if(strcmp(root->Value(),"RaytWorld")==0){
		map.clear();
		TiXmlNode *child = 0;
		TiXmlNode *child2=0;
		TiXmlElement *e,*e2;
		const char *p;
    while( child = root->IterateChildren( child ) ){
			e=child->ToElement();
			if(e && strcmp(e->Value(),"LineSegment")==0){
				TiXmlElement *z;
				LineSeg l;
				child2=0;
				bool firstseg=true;
				while( child2 = e->IterateChildren( child2 ) ){
					e2=child2->ToElement();
					if(e2){
						if(strcmp(e2->Value(),"Color")==0){
							p=e2->Attribute("r");
							l.col.r=(p)?atoi(p):0;
							p=e2->Attribute("g");
							l.col.g=(p)?atoi(p):0;
							p=e2->Attribute("b");
							l.col.b=(p)?atoi(p):0;
						}
						if(strcmp(e2->Value(),"Point")==0){
							Point pt;
							p=e2->Attribute("x");
							pt.x=(p)?atoi(p):0;
							p=e2->Attribute("y");
							pt.y=(p)?atoi(p):0;
							if(firstseg){
								l.start=pt;
								firstseg=false;
							}else{
								l.end=pt;
							}
						}
					}
				}
				map.push_back(l);
			}
		}
	}
}