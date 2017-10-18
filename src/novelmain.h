#pragma once
#include "common.h"
#include "novelheader.h"
#include "vndsparse.h"
#include "imagecontrol.h"
#include "textcontrol.h"
#include "menupanel.h"

class NovelMain
{
private:
	std::string Path;
	vita2d_pgf * pgf = vita2d_load_default_pgf();	//Font!

	//VNDSParser Parser;	
	TextControl Text;
	ForegroundControl Foreground;
	BackgroundControl Background;
	MenuPanel Menu;

	NovelHeader Novel;
	void Tick(SceCtrlData GamePad,SceCtrlData GamePadLast);
	void Draw();
public:
	NovelMain(std::string LoadPath);
	~NovelMain();
	void Run();
};