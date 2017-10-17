#include "vndsparse.h"

VNDSParser::VNDSParser()
{
	Active = false;
}

void VNDSParser::LoadFile(const std::string Path, const std::string File)
{
	Active = true;
	this->Path = Path+"/script";
	this->CurrentScript = this->Path + "/"+ File;

	std::string Line;
	std::ifstream text(CurrentScript);
	if (text.is_open())
	{
		while (text.good())
		{
			getline(text,Line);
			if(!Line.empty())
			{
				
				Script.emplace_back(OpcodeType::Text,Line);
			}	
		}
		text.close();
	}
	else
	{
		Script.emplace_back(OpcodeType::Text,"can't open file!");
	}
}

std::string VNDSParser::RunNextLine()
{
	CurrentLine++;
	if(!IsFinished())
	{
		return Script[CurrentLine].Operand;
	}
	else
	{
		return "oh no, EOF!";
	}
}

void VNDSParser::JumpTo(int LineNo)
{
	CurrentLine = LineNo;
}

bool VNDSParser::IsFinished()
{
	return (CurrentLine >= (int)Script.size());
}