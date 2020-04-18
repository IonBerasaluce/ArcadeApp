#include "FileCommandLoader.h"
#include <fstream>
#include <iostream>

#include "Graphics/Colour.h"
#include "Math/Vec2D.h"

void FileCommandLoader::AddCommand(const Command& command)
{
	m_Command.push_back(command);
}

bool FileCommandLoader::LoadFile(const std::string& filePath)
{
	std::ifstream inFile;

	inFile.open(filePath);

	std::string line = "";

	if (!inFile.is_open())
	{
		std::cout << "Could not open the file: " << filePath << std::endl;
		return false;
	}

	while (!inFile.eof())
	{
		std::getline(inFile, line);

		size_t commandPos = std::string::npos;

		if ((commandPos = line.find(":")) != std::string::npos)
		{
			size_t dilimitPos = line.find_first_of(" ", commandPos);

			if (dilimitPos == std::string::npos)
			{
				dilimitPos = line.length();
			}
			else
			{
				dilimitPos -= 1;
			}

			std::string commandStr = line.substr(commandPos + 1, dilimitPos);
			dilimitPos += 1;

			for (size_t commandIndex = 0; commandIndex < m_Command.size(); ++commandIndex)
			{
				if (commandStr == m_Command[commandIndex].command)
				{
					if (m_Command[commandIndex].commandType == COMMAND_ONE_LINE)
					{
						ParseFunctionParams params;
						params.dilimitPos = dilimitPos;
						params.lineNum = 0;
						params.line = line;
						m_Command[commandIndex].parseFunc(params);
					}
					else
					{
						std::string numLines = line.substr(dilimitPos + 1);
						int totalLines = std::stoi(numLines);
						int lineNum = 0;

						while (lineNum < totalLines)
						{
							std::getline(inFile, line);


							if (line.empty())
							{
								continue;
							}
							else
							{
								ParseFunctionParams params;
								params.dilimitPos = 0;
								params.lineNum = lineNum;
								params.line = line;
								m_Command[commandIndex].parseFunc(params);
								++lineNum;
							}
						}
					}
				}
			}

		}
	}
	return true;
}

Colour FileCommandLoader::ReadColour(const ParseFunctionParams& params)
{
	size_t nextSpacePos = params.line.find_first_of(" ", params.dilimitPos + 1);
	int r = std::stoi(params.line.substr(params.dilimitPos, (nextSpacePos - params.dilimitPos)));

	size_t lastSpacePos = nextSpacePos;
	nextSpacePos = params.line.find_first_of(" ", lastSpacePos + 1);
	int g = std::stoi(params.line.substr(lastSpacePos + 1, nextSpacePos - lastSpacePos));
	
	lastSpacePos = nextSpacePos;
	nextSpacePos = params.line.find_first_of(" ", lastSpacePos + 1);
	int b = std::stoi(params.line.substr(lastSpacePos + 1, nextSpacePos - lastSpacePos));

	int a = std::stoi(params.line.substr(nextSpacePos + 1));

	return Colour(r, g, b, a);
}

Vec2D FileCommandLoader::ReadSize(const ParseFunctionParams& params)
{
	size_t nextSpacePos = params.line.find_first_of(" ", params.dilimitPos + 1);
	int width = std::stoi(params.line.substr(params.dilimitPos, nextSpacePos - params.dilimitPos));
	
	size_t lastSpacePos = nextSpacePos;
	nextSpacePos = params.line.find_first_of(" ", lastSpacePos + 1);
	int height = std::stoi(params.line.substr(lastSpacePos + 1, nextSpacePos - lastSpacePos));
	
	return Vec2D(width, height);
}

int FileCommandLoader::ReadInt(const ParseFunctionParams& params)
{
	std::string intStr = params.line.substr(params.dilimitPos + 1);
	return std::stoi(intStr);
}

std::string FileCommandLoader::ReadString(const ParseFunctionParams& params)
{
	return params.line.substr(params.dilimitPos + 1);
}

char FileCommandLoader::ReadChar(const ParseFunctionParams& params)
{
	return params.line.substr(params.dilimitPos + 1)[0];
}
