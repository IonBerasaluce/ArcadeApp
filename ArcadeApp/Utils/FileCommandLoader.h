#pragma once

#include <functional>
#include <vector>
#include <string>
#include <stdint.h>

class Colour;
class Vec2D;

enum CommandType
{
	COMMAND_ONE_LINE = 0,
	COMMAND_MULTI_LINE
};

struct ParseFunctionParams
{
	std::string line;
	size_t dilimitPos;
	uint32_t lineNum;
};

using ParseFunc = std::function<void(const ParseFunctionParams& params)>;

struct Command
{
	CommandType commandType = COMMAND_ONE_LINE;
	std::string command = "";
	ParseFunc parseFunc = nullptr;
};

class FileCommandLoader
{
public:
	void AddCommand(const Command& command);
	bool LoadFile(const std::string& filePath);

	static Colour ReadColour(const ParseFunctionParams& params);
	static Vec2D ReadSize(const ParseFunctionParams& params);
	static int ReadInt(const ParseFunctionParams& params);
	static std::string ReadString(const ParseFunctionParams& params);
	static char ReadChar(const ParseFunctionParams& params);

private:
	std::vector<Command> m_Command;
};


