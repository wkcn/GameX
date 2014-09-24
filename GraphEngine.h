//---------------------------------------------------------------
// **图像引擎**
// 负责窗口创建、处理图像渲染以及字体渲染
//---------------------------------------------------------------
#pragma once
#include "Defines.h"

//FreeImage
#include <FreeImage.h>

//FreeType
#include <ft2build.h>
//#include FT_FREETYPE_H 
#include <freetype.h>
#include <ftglyph.h>


struct CharTexture
{
	GLuint  m_texID;
	wchar_t m_chaID;
	int     m_Width;
	int     m_Height;

	int     m_adv_x;
	int     m_adv_y;
	int     m_delta_x;
	int     m_delta_y;

	CharTexture()
	{
		m_texID = 0;
		m_chaID = 0;
		m_Width = 0;
		m_Height = 0;
	}
};

struct FontData
{
	int fontBufferSize;
	FT_Face pFace;
	CharTexture* pCharTex;
};

class GraphEngine{

private:
	GLFWwindow* window;
	string _title;
	int _cx, _cy;
	int _width, _height;
	//float ratio_;
	int _maxfps;
	int _upTime;//毫秒单位
	RGBAData color1, color2, color3, color4;
	map<string, Texture*> texMap;

	//FreeType
	FT_Library pFTLib;
	FT_Face   pFTFace;				//当前字体指针
	map<string, FontData>  fontMap; // 这里是按字体名称绑定缓存大小的
	int _fontSize;
	int _fontBufferSize;

private:
	bool CreateWnd();
	bool InitFreeType();
	GLuint LoadChar(wchar_t ch);
public:
	bool Init();
	void Render();
	void PreUpdate();
	void PostUpdate();
	void Record(string title, int cx, int cy, int maxfps);
	void Draw(Texture *tex, Rect drawRectI, Rect srcRectI, float ox, float oy, float angle);
	void Text(string text, int x, int y, float ox, float oy, float angle);
	void SetFont(string fontName,int buffersize = 64);
	void SetFontSize(int size);
	void SetColor(RGBAData &_color1, RGBAData &_color2, RGBAData &_color3, RGBAData &_color4);
	int WindowShouldClose();
	Texture * LoadTexture(const string filename);
public:
	~GraphEngine();
};