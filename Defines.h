#pragma once

#define _CRT_SECURE_NO_WARNINGS

//Sleep��������ͷ�ļ�
#ifdef _MSC_VER
#include <windows.h>
#else
//#include <syswait.h>
#include <unistd.h>
#endif

// ��׼ģ�������
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <list>
#include <fstream>
//#include <hash_map>
#include <iostream>
#include <cmath>
#include <ctime>
#include <sstream>
#include <cstdlib>
using namespace std;

//GLFW
#include <GLFW/glfw3.h>

/*���ش���*///
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
//typedef float GLfloat;

// һЩ��
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define safe_release(p)      { if(p) { (p)->release(); (p)=NULL; } }
#define SAFE_RELEASE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; }}

//����ڴ��Ƿ�й©
#define _CRTDBG_MAP_ALLOC 
#ifdef _DEBUG
_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
// _CrtSetBreakAlloc(5351);
#endif

//����
const double PI = 3.1415926;

void ErrorExit(string msg);
/*
//�򵥵���ʱ
class TimerX
{
private:
	static clock_t last, current;
	double timegap;
	double elapsedTime,intervalTime;
public:
	TimerX(double Int=0){Reset(Int);}//������λ
	void Update()
	{
		current = clock();
		timegap = (current-last)/(double)CLK_TCK;//���뻯��
		last = current;
		elapsedTime -= timegap;
	}
	void Reset(double Int=0)
	{
		last=current=clock();
		intervalTime=Int;
		elapsedTime=Int;
	}
	double GetElapsedTime(bool clear=true)
	{
		Update();
		return elapsedTime;
	}
};*/

struct RGBAData
{
	unsigned char r,g,b,a;
	RGBAData(){r = g = b =a = 255;}
	RGBAData(unsigned char _a):r(255),g(255),b(255),a(_a){};
	RGBAData(unsigned char _r,unsigned char _g,unsigned char _b):r(_r),g(_g),b(_b),a(255){};
	RGBAData(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a):r(_r),g(_g),b(_b),a(_a){};
	void Set(unsigned char _r,unsigned char _g,unsigned char _b){r=_r;g=_g;b=_b;};
	void Set(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a){r=_r;g=_g;b=_b;a=_a;};
	//RGBAData(RGBAData const& d):r(d.r),g(d.g),b(d.b),a(d.a){};
};

typedef struct 
{
	int	width,height;
	GLuint id;
}Texture;

template<typename T>class RectX
{
public:
	T left,top,right,bottom;
	T width,height;
public:
	RectX(T _x,T _y,T w,T h){
		Set(_x,_y,w,h);
	};
	void Set(T _x,T _y,T w,T h){
		left = _x;
		top = _y;
		width = w;
		height = h;
		right = _x+w;
		bottom = _y+h;
	};
};
typedef RectX<int> Rect;

//�ַ�������
string ws2s(const wstring& ws);
wstring s2ws(const string& s);
