#pragma once
#include "common.h"

class NovelHeader
{
	private:
		bool FileExists(std::string Path)
		{
			SceIoStat StatTemp;
			return (sceIoGetstat(Path.c_str(), &StatTemp) >= 0);
		}

		vita2d_texture * LoadImageName(std::string Path)
		{
			//Takes name without type por exampler;  ux0:data/vnvita/saya no uta/icon
			//Priority : High PNG -> High JPG -> Base PNG -> Base PNG

			//Jpeg's don't load and it's pissing me off. Argh!

			vita2d_texture* Texture = NULL;
			std::string PathTemp = Path;

			// High PNG
			PathTemp.append("-high.png");
			if(FileExists(PathTemp))
			{
				Texture = vita2d_load_PNG_file((PathTemp).c_str());
			}
			
			// High JPG
			if(Texture == NULL)
			{
				PathTemp = Path;
				PathTemp.append("-high.jpg");
				if(FileExists(PathTemp))
				{
					Texture = vita2d_load_JPEG_file(PathTemp.c_str());
				}
			}

			// Low PNG
			if(Texture == NULL)
			{
				PathTemp = Path;
				PathTemp.append(".png");
				if(FileExists(PathTemp))
				{
					Texture = vita2d_load_PNG_file(PathTemp.c_str());
				}
			}

			// Low JPG
			if(Texture == NULL)
			{
				PathTemp = Path;
				PathTemp.append(".jpg");
				if(FileExists(PathTemp))
				{
					Texture = vita2d_load_JPEG_file(PathTemp.c_str());
				};
			}

			return Texture;
		}

	public:
	NovelHeader(void);

	NovelHeader(std::string Path, bool LoadImages)
	{
		this->Name = Path;
		this->Path = Path;

		if(LoadImages)
		{
			//Icon
			vita2d_texture * iconPointer = LoadImageName(Path + "\\icon"); 
			if(iconPointer != NULL)
			{
				vita2d_texture_set_filters(iconPointer, SCE_GXM_TEXTURE_FILTER_LINEAR, SCE_GXM_TEXTURE_FILTER_LINEAR);
				this->Icon = std::shared_ptr<vita2d_texture>(iconPointer, vita2d_free_texture);
			}

			//Thumbnail / Preview
			vita2d_texture * thumbPointer = LoadImageName(Path + "\\thumbnail"); 
			if(thumbPointer != NULL)
			{	
				vita2d_texture_set_filters(thumbPointer, SCE_GXM_TEXTURE_FILTER_LINEAR, SCE_GXM_TEXTURE_FILTER_LINEAR);
				this->Thumbnail = std::shared_ptr<vita2d_texture>(thumbPointer, vita2d_free_texture);
			}
		}
	}	

	std::string Name;
	std::string Path;
	std::shared_ptr<vita2d_texture> Icon;
	std::shared_ptr<vita2d_texture> Thumbnail;
};