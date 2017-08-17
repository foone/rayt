void Render();
void Setup(int width, int height);
void RenderUI();

extern SDL_Rect raybox,topbox;

void ResetViewport();

void DrawTopMap();

void DrawSeg(const LineSeg &s);
void DrawSegRed(const LineSeg &s);

void DrawLines();

void Sleep();
void WakeUp();

void SetRes(int lines);

void ShiftR(int amt);
void ShiftG(int amt);
void ShiftB(int amt);

int GetSegmentAtPointer();

void SetSelected(int sel);

void DrawPlayerOut();