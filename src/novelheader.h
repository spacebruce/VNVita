#pragma once
#include "common.h"
#include "../inih/cpp/INIReader.h"

class NovelHeader
{
	private:
		void SetPaths()
		{
			switch(Type)
			{
				case NovelType::Error:
				{
					PathScript = "";
				}; break;
				
				case NovelType::VNDS:
				case NovelType::VNVita:
				{
					PathScript = Path+"\\Script\\main.scr";
				}; break;
			}
		}

		bool FileExists(std::string Path)
		{
			SceIoStat StatTemp;
			return (sceIoGetstat(Path.c_str(), &StatTemp) >= 0);
		}

		vita2d_texture * LoadImageName(std::string Path)
		{
			//Takes name without type. por exampler;  ux0:data/vnvita/saya no uta/icon
			//Priority : High PNG -> High JPG -> Base PNG -> Base JPG
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
		int Width = 256;
		int Height = 192;
		std::string Name;
		std::string Path;
		std::string PathScript;
		NovelType Type;
		std::shared_ptr<vita2d_texture> Icon;
		std::shared_ptr<vita2d_texture> Thumbnail;

		NovelHeader(void) {}
		NovelHeader(std::string Path)
		{
			Reset(Path);
		}

		void Reset(std::string Path)
		{
			this->Name = Path;
			this->Path = Path;
			Type = NovelType::Error;

			//VNVita mode
			if(FileExists(Path+"\\vnvita.ini"))
			{
				Type = NovelType::VNVita;
				Name = "error - bad config";

				INIReader Reader = INIReader(Path + "\\vnvita.ini");
				if(Reader.ParseError() >= 0)
				{
					Name = Reader.Get("","title","error - no title");
					Width = Reader.GetInteger("","width",960);
					Height = Reader.GetInteger("","height",544);
				}

				vita2d_texture * iconPointer = LoadImageName(Path + "\\icon");
				vita2d_texture * thumbPointer= LoadImageName(Path + "\\thumbnail");
				if(iconPointer != NULL)
				{
					vita2d_texture_set_filters(iconPointer, SCE_GXM_TEXTURE_FILTER_LINEAR, SCE_GXM_TEXTURE_FILTER_LINEAR);
					this->Icon = std::shared_ptr<vita2d_texture>(iconPointer, vita2d_free_texture);
				}
				if(thumbPointer != NULL)
				{
					vita2d_texture_set_filters(thumbPointer, SCE_GXM_TEXTURE_FILTER_LINEAR, SCE_GXM_TEXTURE_FILTER_LINEAR);
					this->Thumbnail = std::shared_ptr<vita2d_texture>(thumbPointer, vita2d_free_texture);
				}
			}

			//VNDS mode
			if((FileExists(Path+"\\info.txt")) && (FileExists(Path+"\\img.ini")))
			{
				Type = NovelType::VNDS;

				//Load name from info.txt
				auto InfoPath = Path + "\\info.txt";
				INIReader reader = INIReader(InfoPath);
				if (reader.ParseError() < 0) {
					Name = Path;
					Name.append("#");
				}
				else
				{
					Name = reader.Get("", "title", Path);
				}
				//Image size from img.ini
				auto ImgPath = Path + "\\img.ini";
				reader = INIReader(ImgPath);
				if (reader.ParseError() >= 0)
				{
					Width = reader.GetInteger("", "width", Width);
					Height = reader.GetInteger("", "height", Height);
				}

				//Images
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
};