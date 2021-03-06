#include "GameX.h"

GameX::GameX(string title, int cx, int cy, int maxfps)
{
	_title = title;
	_cx = cx; _cy = cy;
	_maxfps = maxfps;
	_upTime = 1000 / maxfps - 1;
	_graph.Record(title, cx, cy, maxfps);//传递参数到图像引擎
	Init();
}
GameX::~GameX()
{

}
bool GameX::Init()
{
	if (!_graph.Init())
		ErrorExit("图像引擎初期化失败");
	if (!_sound.Init())
		ErrorExit("图像引擎初期化失败");
	return true;
}
void GameX::Run()
{

	//static clock_t lastuTime, lastfTime, currentTime;
	//lastuTime = lastfTime = currentTime = clock();
	int count = 0;
	long restTime = 0, restFPSTime = 0;
	fps = 0;
	clock_t currentTime, lastFPSTime, lastRenderTime;
	currentTime = lastFPSTime = lastRenderTime = clock();

	//Begin();

	while (!_graph.WindowShouldClose())
	{
		currentTime = clock();
		//restTime = _upTime - (currentTime - lastuTime);
		restTime = _upTime - (currentTime - lastRenderTime) * 1000 / CLOCKS_PER_SEC;

		if (restTime < 0)
		{

			//更新音频
			_sound.Update();

			//更新画面
			_graph.PreUpdate();

			//游戏处理更新
			Update();

			//stringstream temp;
			//temp << "FPS:" << fps;
			//Text(temp.str(),0,0);

			_graph.PostUpdate();

			++count;

			lastRenderTime = currentTime;

			restFPSTime = currentTime - lastFPSTime;

			restFPSTime = restFPSTime * 1000 / CLOCKS_PER_SEC;

			//cout<<restFPSTime<<" "<<count<<" "<<CLOCKS_PER_SEC<<endl;

			if (restFPSTime >= 1000)
			{
				fps = int(count*1000.0 / restFPSTime);
				//if(fps>_maxfps)fps = _maxfps;
				count = 0;
				lastFPSTime = currentTime;
			}
		}
		else
		{
#ifdef _MSC_VER
			//Sleep(2);
#else
			//usleep(2000);
#endif
		}
	}
	
	//End();

}

//void GameX::Begin(){};
//void GameX::End(){};

void GameX::Render()
{
	_graph.Render();
}

int GameX::GetFPS()
{
	return fps;
}

Texture * GameX::LoadTexture(const string & filename){
	return _graph.LoadTexture(filename);
}
void GameX::SetColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
	SetColor(RGBAData(_r, _g, _b, _a));
}
void GameX::SetColor(RGBAData _color /* = RGBAData */)
{
	_graph.SetColor(_color, _color, _color, _color);
}
void GameX::SetColor(RGBAData _color1,RGBAData _color2,RGBAData _color3,RGBAData _color4)
{
	_graph.SetColor(_color1, _color2, _color3, _color4);
}

void GameX::Draw(string filename,int x,int y,float ox,float oy,float angle)
{
	Texture* tex = LoadTexture(filename);
	_graph.Draw(tex,Rect(int(x-ox),int(y-oy),tex->width,tex->height),Rect(0,0,tex->width,tex->height),ox,oy,angle);
}
void GameX::Draw(string filename,int x,int y,Rect srcRect,float ox,float oy,float angle)
{
	Texture* tex = LoadTexture(filename);
	_graph.Draw(tex,Rect(int(x-ox),int(y-oy),tex->width,tex->height),srcRect,ox,oy,angle);
}
void GameX::Draw(string filename,Rect drawRect,float ox,float oy,float angle)
{
	Texture* tex = LoadTexture(filename);
	_graph.Draw(tex,drawRect,Rect(0,0,tex->width,tex->height),ox,oy,angle);
}
void GameX::Draw(string filename,Rect drawRect,Rect srcRect,float ox,float oy,float angle)
{
	Texture* tex = LoadTexture(filename);
	_graph.Draw(tex,drawRect,srcRect,ox,oy,angle);
}
void GameX::Draw(Texture *tex,int x,int y,float ox,float oy,float angle)
{
	_graph.Draw(tex,Rect(int(x-ox),int(y-oy),tex->width,tex->height),Rect(0,0,tex->width,tex->height),ox,oy,angle);
}
void GameX::Draw(Texture *tex,int x,int y,Rect srcRect,float ox,float oy,float angle)
{
	_graph.Draw(tex,Rect(int(x-ox),int(y-oy),tex->width,tex->height),srcRect,ox,oy,angle);
}
void GameX::Draw(Texture *tex,Rect drawRect,float ox,float oy,float angle)
{
	_graph.Draw(tex,drawRect,Rect(0,0,tex->width,tex->height),ox,oy,angle);
}
void GameX::Draw(Texture *tex,Rect drawRectI,Rect srcRectI,float ox,float oy,float angle)
{
	_graph.Draw(tex, drawRectI, srcRectI, ox, oy, angle);
}


void GameX::Text(string text, int x, int y, float ox /* = 0 */, float oy /* = 0 */, float angle /* = 0 */)
{
	_graph.Text(text, x, y, ox, oy, angle);
}
void GameX::SetFont(string fontName, int buffersize /* = 64 */)
{
	_graph.SetFont(fontName, buffersize);
}
void GameX::SetFontSize(int size)
{
	_graph.SetFontSize(size);
}
void GameX::Play(string filename, CHANNEL c, float volume /* = 100.0f */, bool loop /* = true */)
{
	_sound.Play(filename, c, volume, loop);
}
void GameX::PlaySE(string filename, float volume /* = 100.0f */)
{
	_sound.PlaySE(filename, volume);
}
void GameX::StopAllSound()
{
	_sound.StopAllSound();
}
void GameX::StopChannel(CHANNEL c)
{
	if (c == -1)
	{
		StopAllSound();
		return;
	}
	_sound.StopChannel(c);
}
