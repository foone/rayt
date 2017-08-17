struct Color{
	unsigned char r,g,b;
	Color(unsigned char nr,unsigned char ng, unsigned char nb){r=nr;g=ng;b=nb;}
	Color(){};
};
struct Point{
	double x,y;
	Point(){};
	Point(float nx,float ny):x(nx),y(ny){}
};
struct LineSeg{
	Point start,end;
	Color col;
	LineSeg(){}
	LineSeg(float sx,float sy,float ex,float ey):
	start(sx,sy),end(ex,ey){}
	
};
void Exit(int code);


#define PI 3.141592654f