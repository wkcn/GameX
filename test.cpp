#include "test.h"

GameMain::GameMain(string title, int cx, int cy, int maxfps) :GameX(title, cx, cy, maxfps){
	//随机数种子
	srand(clock());
	SetFontSize(20);
	//Play("cc.mp3", 0);
	//cout << "music" << endl;
};

void GameMain::Update()
{
	SetColor(RGBAData(255, 255, 255));
	stringstream temp;
	temp << "FPS:" << GetFPS();
	Text(temp.str(),0,10);
}
