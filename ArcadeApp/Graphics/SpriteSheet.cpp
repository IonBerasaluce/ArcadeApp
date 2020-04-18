#include "SpriteSheet.h"
#include "Utils/FileCommandLoader.h"
#include "Utils/Utils.h"
#include "App/App.h"

SpriteSheet::SpriteSheet()
{
}

bool SpriteSheet::Load(const std::string& name)
{
	bool loadedImage = m_BMPImage.Load(App::Singleton().GetBasePath() + std::string("Assets\\" + name + ".bmp"));
	bool loadedSpriteSections = LoadSpriteSections(App::Singleton().GetBasePath() + std::string("Assets\\" + name + ".txt"));

	return loadedImage && loadedSpriteSections;
}

Sprite SpriteSheet::GetSprite(const std::string& spriteName) const
{
	size_t length = m_Sections.size();
	for (size_t i = 0; i < length; i++)
	{
		if (StringCompare(m_Sections[i].key, spriteName))
		{
			return m_Sections[i].sprite;
		}
	}

	return Sprite();
}

std::vector<std::string> SpriteSheet::SpriteNames() const
{

	std::vector<std::string> spriteNames;

	for (const auto& section : m_Sections)
	{
		spriteNames.push_back(section.key);
	}
	
	return  spriteNames;
}

bool SpriteSheet::LoadSpriteSections(const std::string& path)
{
	FileCommandLoader fileLoader;

	Command spriteCommand;
	spriteCommand.command = "sprite";
	spriteCommand.parseFunc = [&](ParseFunctionParams params)
	{
		BMPImageSection section;
		m_Sections.push_back(section);
	};

	fileLoader.AddCommand(spriteCommand);

	Command keyCommand;
	keyCommand.command = "key";
	keyCommand.parseFunc = [&](ParseFunctionParams params)
	{
		m_Sections.back().key = FileCommandLoader::ReadString(params);
	};

	fileLoader.AddCommand(keyCommand);

	Command xPosCommand;
	xPosCommand.command = "xPos";
	xPosCommand.parseFunc = [&](ParseFunctionParams params)
	{
		m_Sections.back().sprite.xPos = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(xPosCommand);

	Command yPosCommand;
	yPosCommand.command = "yPos";
	yPosCommand.parseFunc = [&](ParseFunctionParams params)
	{
		m_Sections.back().sprite.yPos = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(yPosCommand);

	Command widthCommand;
	widthCommand.command = "width";
	widthCommand.parseFunc = [&](ParseFunctionParams params)
	{
		m_Sections.back().sprite.width = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(widthCommand);

	Command heightCommand;
	heightCommand.command = "height";
	heightCommand.parseFunc = [&](ParseFunctionParams params)
	{
		m_Sections.back().sprite.height = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(heightCommand);

	return fileLoader.LoadFile(path);

}
