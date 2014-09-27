#include "GraphEngine.h"

CharTexture *charTexID;

bool GraphEngine::Init(){

	FreeImage_Initialise();

	if (!glfwInit())
		return false;

	char text[64];
	strcpy(text, _title.c_str());

	glfwWindowHint(GLFW_DECORATED, 0);
	//glfwWindowHint(GLFW_SAMPLES, 1);
	//glfwWindowHint(GLFW_VISIBLE, 0);
	glfwWindowHint(GLFW_STEREO, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 0);


	window = glfwCreateWindow(_cx, _cy, text, NULL, NULL);

//	window->win32.dwStyle &= WS_EX_TOOLWINDOW;
	//glfwSetWindowTitle(window, "你好啊");
	//glfwGetWindowAttrib(window, GLFW_FOCUSED);

	//int a, b;
	//glfwGetFramebufferSize(window, &a, &b); 1440x900
	GLFWmonitor * monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	//居中
	//int x, y;
	//x = (mode->width - _cx) / 2;
	//y = (mode->height - _cy) / 2;
	//glfwSetWindowPos(window,x, y);
	glfwSetWindowPos(window, 0, 0);

	if (!window)
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	//打开2D贴图状态
	glEnable(GL_TEXTURE_2D);
	//混色
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);

	//抗锯齿
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glHint(GL_FOG_HINT, GL_NICEST);



	//FreeType
	SetFont("default.ttf", 64);
	_fontSize = 64;
	_fontBufferSize = 64;

	return true;
}
void GraphEngine::Record(string title, int cx, int cy, int maxfps){
	_title = title;
	_cx = cx; _cy = cy;
	_maxfps = maxfps;
	_upTime = 1000 / maxfps - 1;
}
GLFWwindow * GraphEngine::GetGLFWwindow(){
	return window;
}
void GraphEngine::PreUpdate(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Render();
}
void GraphEngine::PostUpdate(){
	glfwSwapBuffers(window);
	glfwPollEvents();
}
int GraphEngine::WindowShouldClose(){
	return glfwWindowShouldClose(window);
}
void GraphEngine::Render(){

	glfwGetFramebufferSize(window, &_width, &_height);
	//ratio_ = width_ / (float) height_;
	glViewport(0, 0, _width, _height);

	//投影变换
	glMatrixMode(GL_PROJECTION);
	//单位矩阵
	glLoadIdentity();

	//glOrtho(-ratio_, ratio_, -1.f, 1.f, 1.f, -1.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glScalef(2.f / _cx, 2.0f / _cy, 1);
	glRotatef(180, 1, 0, 0);
	glTranslatef(float(-_cx / 2), float(-_cy / 2), 0);

}

void GraphEngine::SetFont(string fontName, int buffersize){

	map<string, FontData>::iterator iter;
	iter = fontMap.find(fontName);
	if (iter != fontMap.end())
	{
		pFTFace = iter->second.pFace;
		_fontBufferSize = iter->second.fontBufferSize;
		charTexID = iter->second.pCharTex;
		if (buffersize == _fontBufferSize)
		{
			return;
		}
		else
		{
			//释放字体（未写！）
		}
	}

	if (FT_Init_FreeType(&pFTLib))return;

	//加载一个字体,取默认的Face,一般为Regualer
	if (FT_New_Face(pFTLib, fontName.c_str(), 0, &pFTFace))return;
	FT_Select_Charmap(pFTFace, FT_ENCODING_UNICODE);
	//pFTFace->num_fixed_sizes;
	//大小要乘64.这是规定。照做就可以了。
	//FT_Set_Char_Size( m_FT_Face , 0 , m_w << 6, 96, 96);
	FT_Set_Pixel_Sizes(pFTFace, buffersize, buffersize);

	charTexID = new CharTexture[65536];
	fontMap[fontName].pFace = pFTFace;
	fontMap[fontName].fontBufferSize = _fontBufferSize;
	fontMap[fontName].pCharTex = charTexID;
}

void GraphEngine::SetFontSize(int size){
	_fontSize = size;
}

Texture * GraphEngine::LoadTexture(const string filename){

	map<string, Texture*>::iterator iter;

	iter = texMap.find(filename);

	if (iter != texMap.end())
	{
		//从纹理池找到纹理
		return iter->second;
	}


	char textName[128];
	strcpy(textName, filename.c_str());

	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to
	GLuint gl_texID;

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(textName, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(textName);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		ErrorExit("未知类型/n打开" + filename + "文件出错 >.<");

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, textName);
	//if the image failed to load, return failure
	if (!dib)
		ErrorExit("无法读取/n打开" + filename + "文件出错 >.<");


	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		ErrorExit("读取图像数据出错\n打开" + filename + "文件出错 >.<");

	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &gl_texID);

	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, gl_texID);

	GLenum image_format = GL_RGB;		//format the image is in
	GLint internal_format = GL_BGR_EXT;	//format to store the image in

	int bpp = FreeImage_GetBPP(dib);
	if (bpp == 32)//(fif == FIF_PNG)
	{
		image_format = GL_RGBA;
		internal_format = GL_BGRA_EXT;
	}


	glTexImage2D(GL_TEXTURE_2D, 0, image_format, width, height,
		0, internal_format, GL_UNSIGNED_BYTE, bits);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP);
	//线性滤波
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	Texture *newTex = new Texture;
	newTex->id = gl_texID;
	newTex->width = width;
	newTex->height = height;
	texMap[filename] = newTex;

	return newTex;
}
void GraphEngine::SetColor(RGBAData &_color1, RGBAData &_color2, RGBAData &_color3, RGBAData &_color4){
	color1 = _color1;
	color2 = _color2;
	color3 = _color3;
	color4 = _color4;
}
void GraphEngine::Draw(Texture *tex, Rect drawRectI, Rect srcRectI, float ox, float oy, float angle)
{

	//平面坐标系算法
	// x=(x1-x2)cosθ-(y1-y2)sinθ+x2	y=(y1-y2)cosθ+(x1-x2)sinθ+y2

	double ang = angle / 180 * PI;

	float _cos = float(cos(ang));
	float _sin = float(sin(ang));

	//矩形标准化
	RectX<float> drawRect(float(drawRectI.left), float(drawRectI.top), float(drawRectI.width), float(drawRectI.height));
	RectX<float> srcRect(srcRectI.left*1.0f / tex->width, (tex->height - srcRectI.bottom)*1.0f / tex->height, srcRectI.width*1.0f / tex->width, srcRectI.height*1.0f / tex->height);

	ox += drawRect.left;
	oy += drawRect.top;

	//象限顺序处理法 多个坐标系烦死了 >.<
	//窗口坐标系算法

	float pos[8] =
	{

		/* int x1 = */(drawRect.right - ox) * _cos + (drawRect.top - oy) * _sin + ox,
		/* int y1 = */(drawRect.top - oy) * _cos - (drawRect.right - ox) * _sin + oy,

		/*int x2 = */(drawRect.left - ox) * _cos + (drawRect.top - oy) * _sin + ox,
		/*int y2 = */(drawRect.top - oy) * _cos - (drawRect.left - ox) * _sin + oy,

		/*int x3 = */(drawRect.left - ox) * _cos + (drawRect.bottom - oy) * _sin + ox,
		/*int y3 = */(drawRect.bottom - oy) * _cos - (drawRect.left - ox) * _sin + oy,

		/*int x4 = */(drawRect.right - ox) * _cos + (drawRect.bottom - oy) * _sin + ox,
		/*int y4 = */(drawRect.bottom - oy) * _cos - (drawRect.right - ox) * _sin + oy,

	};

	glBindTexture(GL_TEXTURE_2D, tex->id);

	//渲染
	glBegin(GL_QUADS);

	//按平面坐标系象限顺序
	//右上，左上，左下，右下

	glColor4ub(color1.r, color1.g, color1.b, color1.a);
	glTexCoord2d(srcRect.right, srcRect.bottom);
	glVertex3f(pos[0], pos[1], 0);

	glColor4ub(color2.r, color2.g, color2.b, color2.a);
	glTexCoord2d(srcRect.left, srcRect.bottom);
	glVertex3f(pos[2], pos[3], 0);

	glColor4ub(color3.r, color3.g, color3.b, color3.a);
	glTexCoord2d(srcRect.left, srcRect.top);
	glVertex3f(pos[4], pos[5], 0);

	glColor4ub(color4.r, color4.g, color4.b, color4.a);
	glTexCoord2d(srcRect.right, srcRect.top);
	glVertex3f(pos[6], pos[7], 0);

	glEnd();

}


GLuint GraphEngine::LoadChar(wchar_t ch)
{
	if (charTexID[ch].m_texID)
		return charTexID[ch].m_texID;

	if (FT_Load_Char(pFTFace, ch, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT |
		(true ? FT_LOAD_TARGET_NORMAL : FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO)))
	{
		return 0;
	}

	CharTexture& charTex = charTexID[ch];

	//得到字模
	FT_Glyph glyph;
	if (FT_Get_Glyph(pFTFace->glyph, &glyph))
		return 0;

	//转化成位图
	FT_Render_Glyph(pFTFace->glyph, FT_RENDER_MODE_LCD);//FT_RENDER_MODE_NORMAL  ); 
	///////////FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//取道位图数据
	FT_Bitmap& bitmap = bitmap_glyph->bitmap;
	/*
	for ( int  i = 0 ; i < bitmap.rows;  ++ i)
	{
	for ( int  j = 0 ; j < bitmap.width;  ++ j)
	{
	//  if it has gray>0 we set show it as 1, o otherwise
	printf( "%d" , bitmap.buffer[i * bitmap.width + j] ? 1 : 0 );
	}
	printf( " \n " );
	}  */

	//把位图数据拷贝自己定义的数据区里.这样就可以画到需要的东西上面了。
	int width = bitmap.width;
	int height = bitmap.rows;

	//pFTFace->size->metrics.y_ppem;
	//pFTFace->glyph->metrics.horiAdvance;


	charTex.m_Width = width;
	charTex.m_Height = height;
	charTex.m_adv_x = pFTFace->glyph->advance.x / 64;//64.0f;
	charTex.m_adv_y = pFTFace->size->metrics.y_ppem; //m_FT_Face->glyph->metrics.horiBearingY / 64.0f;
	charTex.m_delta_x = bitmap_glyph->left;
	charTex.m_delta_y = bitmap_glyph->top - height;
	glGenTextures(1, &charTex.m_texID);
	glBindTexture(GL_TEXTURE_2D, charTex.m_texID);
	char* pBuf = new char[width * height * 4];
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			unsigned char _vl = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width*j];
			pBuf[(4 * i + (height - j - 1) * width * 4)] = 0xff;
			pBuf[(4 * i + (height - j - 1) * width * 4) + 1] = 0xff;
			pBuf[(4 * i + (height - j - 1) * width * 4) + 2] = 0xff;
			pBuf[(4 * i + (height - j - 1) * width * 4) + 3] = _vl;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBuf);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	delete[]pBuf;
	return charTex.m_chaID;
}

void GraphEngine::Text(string text, int x, int y, float ox, float oy, float angle){
	setlocale(LC_ALL, "chs");
	const char * source = text.c_str();
	size_t dsize = text.size() + 1;
	wchar_t * dest = new wchar_t[dsize];
	wmemset(dest, 0, dsize);
	mbstowcs(dest, source, dsize);
	wstring strText = dest;
	delete[] dest;
	setlocale(LC_ALL, "C");

	int ch_x = x;
	int ch_y = y;
	int w, h;
	float fox, foy;
	fox = ox + x;
	foy = oy + y;
	for (unsigned int i = 0; i < strText.length(); i++)
	{
		LoadChar(strText[i]);
		CharTexture * pCharTex = &charTexID[strText[i]];
		glBindTexture(GL_TEXTURE_2D, pCharTex->m_texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		w = pCharTex->m_Width * _fontSize / _fontBufferSize;
		h = pCharTex->m_Height * _fontSize / _fontBufferSize;

		ch_y = y + _fontSize - h - pCharTex->m_delta_y * _fontSize / _fontBufferSize;

		RectX<float> drawRect(float(ch_x), float(ch_y), float(w), float(pCharTex->m_Height * _fontSize / _fontBufferSize));

		double ang = angle / 180 * PI;

		float _cos = float(cos(ang));
		float _sin = float(sin(ang));

		//旋转
		float pos[8] =
		{

			/* int x1 = */(drawRect.right - fox) * _cos + (drawRect.top - foy) * _sin + fox,
			/* int y1 = */(drawRect.top - foy) * _cos - (drawRect.right - fox) * _sin + foy,

			/*int x2 = */(drawRect.left - fox) * _cos + (drawRect.top - foy) * _sin + fox,
			/*int y2 = */(drawRect.top - foy) * _cos - (drawRect.left - fox) * _sin + foy,

			/*int x3 = */(drawRect.left - fox) * _cos + (drawRect.bottom - foy) * _sin + fox,
			/*int y3 = */(drawRect.bottom - foy) * _cos - (drawRect.left - fox) * _sin + foy,

			/*int x4 = */(drawRect.right - fox) * _cos + (drawRect.bottom - foy) * _sin + fox,
			/*int y4 = */(drawRect.bottom - foy) * _cos - (drawRect.right - fox) * _sin + foy,

		};

		glBegin(GL_QUADS);
		{
			glColor4ub(color1.r, color1.g, color1.b, color1.a);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(pos[0], pos[1], 0);
			glColor4ub(color2.r, color2.g, color1.b, color2.a);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(pos[2], pos[3], 0);
			glColor4ub(color3.r, color3.g, color3.b, color3.a);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(pos[4], pos[5], 0);
			glColor4ub(color4.r, color4.g, color4.b, color4.a);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(pos[6], pos[7], 0);
		}

		ch_x += w + pCharTex->m_delta_x * _fontSize / _fontBufferSize;

		glEnd();
	}
}

GraphEngine::~GraphEngine(){

	glfwDestroyWindow(window);
	glfwTerminate();

	map<string, Texture*>::iterator  iter;
	for (iter = texMap.begin(); iter != texMap.end(); iter++)
	{
		SAFE_DELETE(iter->second);
	}
}
