class Player{
	private:
		bool key_up,key_down,key_left,key_right,key_run;
	public:
		float x,y,rot;
		Player();
		~Player();
		void KeyUp(bool pressed){key_up=pressed;}
		void KeyDown(bool pressed){key_down=pressed;}
		void KeyLeft(bool pressed){key_left=pressed;}
		void KeyRight(bool pressed){key_right=pressed;}
		void KeyRun(bool pressed){key_run=pressed;}
		void GoHome(){x=0;y=0;}
		void Move();
		void Draw();
		void ZDraw(float nx,float ny);
		LineSeg LeftFov;
		LineSeg RightFov;
		LineSeg ForwardSeg;
};

