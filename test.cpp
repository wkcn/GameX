#include "test.h"

GameMain::GameMain(string title, int cx, int cy, int maxfps) :GameX(title, cx, cy, maxfps){
	srand(clock());
	Play("cc.mp3", 0);
	cout << "music" << endl;
};

void GameMain::Update()
{
	
	//2D渲染
	Texture* tt = LoadTexture("qb.jpg");
	static float k=0;
	k++;
	SetColor(RGBAData(255,255,200,200));
	Draw(tt,Rect(100,100,200,200),Rect(0,0,tt->width,443),0,0,k);
	SetColor();
	
	Texture* miku2 = LoadTexture("miku.jpg");
	SetColor(RGBAData(200));
	Draw(miku2,Rect(200,300,250,280));
	SetColor();
	
	//SetColor(RGBAData(100,200,50));
	//SetFontSize(32);
	//Text("我爱我的祖国chi",0,0,0,0,-45);
	//Text("就像花儿一样", 300, 500,0,0);

	Draw("Particle.png", 0, 0, 25, 20, 40);

	for (vector<Maple>::iterator iter = maples.begin(); iter != maples.end();)
	{
		SetColor(RGBAData(iter->c.r, iter->c.g, iter->c.b, iter->life));
		Draw("Particle.png", iter->x, iter->y, 25, 20, iter->angle);
		iter->angle += 3;
		iter->life-=5;
		iter->v++;
		iter->y += iter->v;
		if (iter->life <= 0)
		{
			//maples.erase(iter++);
			iter->life = 255;
			iter->x = rand() % 1000 * 1.440;
			iter->y = 0;
			if (iter->v>20)
			{
				iter->v = 1;
				/*
				iter->c.r -= rand() % 100;
				if (iter->c.r < 0)iter->c.r = 0;
				if (iter->c.r == 0)iter->c.g -= rand() % 100;
				if (iter->c.g < 0)iter->c.g = 0;
				if (iter->c.g == 0)iter->c.b -= rand() % 100;
				if (iter->c.b < 0){ iter->c.b = 0; iter->c.r = iter->c.g = iter->c.b = 255; }
				*/
			}
			iter++;

		}
		else
			iter++;
	}
	for(int i = 0;i<3;i++)
	{
		Maple w;
		w.life = 255; w.angle = rand()%100 * 3.60; w.y = 0; w.x = rand()%1000 * 1.440; w.v = 1;
		w.c.a = w.c.g = w.c.b = 255;
		maples.push_back(w);
	}
	
	SetColor(RGBAData(255, 255, 255));
	stringstream temp;
	temp << "FPS:" << GetFPS();
	Text(temp.str(),0,0);
	stringstream temp2;
	temp2 << "Number of Maples:" << maples.size();
	Text(temp2.str(), 0, 64);
}
