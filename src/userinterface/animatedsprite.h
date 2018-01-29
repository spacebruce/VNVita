#pragma once
#include <vita2d.h>
#include <string>
#include <memory>
#include <vector>
#include "timer.h"

class UIAnimatedSprite
{
private:
	using SpriteType = std::shared_ptr<vita2d_texture>;
	std::vector<SpriteType> FrameList;
	Timer frameTimer = Timer(1000 * 1000);
public:
	uint16_t FrameCurrent = 0;
	uint32_t FrameMilliseconds = 65;

	UIAnimatedSprite(void) = default;
	UIAnimatedSprite(const std::string& firstImage,int FrameMilliseconds);
	UIAnimatedSprite(std::initializer_list<std::string> initialiser);
	UIAnimatedSprite(SpriteType Texture);
	UIAnimatedSprite(std::initializer_list<SpriteType> Texture);

	void SetFrameTime(uint64_t FrameMilliseconds);

	void LoadFrameFromFile(const std::string& Path);
	int GetFrameCount(void) const;

	void Draw(int X,int Y);
	void DrawFrame(int X,int Y,uint16_t Frame);
};