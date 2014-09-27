#pragma once

#include "GraphEngine.h"
#include "SoundEngine.h"

enum{KEY_RELEASE,KEY_PRESS,KEY_REPEAT};

class GameX{
private:

	string _title;
	int _cx, _cy;
	int _width, _height;
	int _maxfps;
	int _upTime;
	int fps;

	GraphEngine _graph;
	SoundEngine _sound;
private:
		bool Init();
public:
	Texture * LoadTexture(const string & filename);

	// 绘制图像
	// 四色顶点按坐象限号排序
	
	void SetColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255);
	void SetColor(RGBAData _color = RGBAData());
	void SetColor(RGBAData _color1,RGBAData _color2,RGBAData _color3,RGBAData _color4);

	void Draw(string filename,Rect drawRect,float ox = 0,float oy = 0,float angle = 0);
	void Draw(string filename,Rect drawRect,Rect srcRect,float ox = 0,float oy = 0,float angle = 0);
	void Draw(string filename,int x,int y,Rect srcRect,float ox = 0,float oy = 0,float angle = 0);
	void Draw(string filename,int x,int y,float ox = 0,float oy = 0,float angle = 0);

	void Draw(Texture *tex,Rect drawRect,float ox = 0,float oy = 0,float angle = 0);
	void Draw(Texture *tex,int x,int y,Rect srcRect,float ox = 0,float oy = 0,float angle = 0);
	void Draw(Texture *tex,int x,int y,float ox = 0,float oy = 0,float angle = 0);

	void Draw(Texture *tex,Rect drawRectI,Rect srcRectI,float ox = 0,float oy = 0,float angle = 0);

	/* 手动渲染处理 */
	//每次调用Opengl函数后用Render()是个好习惯，恢复了GameX的2D模式 >o<
	void Render();

	// 绘制文字
	void Text(string text,int x,int y,float ox = 0,float oy = 0,float angle = 0);
	void SetFont(string fontName, int buffersize = 64);
	void SetFontSize(int size);

	//播放音频
	void PlaySE(string filename, float volume = 100.0f);
	void Play(string filename, CHANNEL c = 0, float volume = 100.0f, bool loop = true);
	void StopAllSound();
	void StopChannel(CHANNEL c = -1);

	//操作函数
	int GetFPS();
	int Key(int keyid);
public:
	void Run();

	virtual void Update() = 0;
	//virtual void Begin()  = 0;
	//virtual void End()    = 0;

	GameX(string title, int cx, int cy, int maxfps =60);//,int argc, char *argv[]);
	~GameX(void);
};
