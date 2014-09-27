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

	// ����ͼ��
	// ��ɫ���㰴�����޺�����
	
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

	/* �ֶ���Ⱦ���� */
	//ÿ�ε���Opengl��������Render()�Ǹ���ϰ�ߣ��ָ���GameX��2Dģʽ >o<
	void Render();

	// ��������
	void Text(string text,int x,int y,float ox = 0,float oy = 0,float angle = 0);
	void SetFont(string fontName, int buffersize = 64);
	void SetFontSize(int size);

	//������Ƶ
	void PlaySE(string filename, float volume = 100.0f);
	void Play(string filename, CHANNEL c = 0, float volume = 100.0f, bool loop = true);
	void StopAllSound();
	void StopChannel(CHANNEL c = -1);

	//��������
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
