extern std::vector<LineSeg> map;

void MapInit();

void CreateBox(int x,int y,int w,int h,Color c);
void CreateShadedBox(int x,int y,int w,int h,Color ns,Color ew);

bool __inline LineSegIntersect(const LineSeg &a,const LineSeg &b,Point &p);

double GetDist(Point &a,Point &b);

void Raytrace();

extern double maxclip;
extern double fov;
extern double foglimit;
void DumpToFile(const char *name);
void LoadMapFromFile(const char *name);
int TestAgainstMap(LineSeg &c);

Point GetOutsidePlayerClosestWallPoint();


bool IsIn(float x,float y);