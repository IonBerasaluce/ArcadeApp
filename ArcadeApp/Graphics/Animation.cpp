#include "Animation.h"
#include "Utils/FileCommandLoader.h"

#include <assert.h>

Animation::Animation(): m_Size(Vec2D::Zero), m_SpriteSheetName(""), m_AnimationName(""), m_FPS(0)
{
}

AnimationFrame Animation::GetAnimationFrame(uint32_t frameNum) const
{
	AnimationFrame frame;

	if (frameNum > m_Frames.size())
	{
		return frame;
	}
	
	frame.frame = m_Frames[frameNum];

	if (frameNum < m_FrameColours.size())
	{
		frame.frameColour = m_FrameColours[frameNum];
		frame.frameColourSet = 1;
	}

	if (m_Overlay.size() > 0)
	{
		frame.overlay = m_Overlay;
	}

	if (frameNum < m_OverlayColours.size())
	{
		frame.overlayColour = m_OverlayColours[frameNum];
	}

	if (frameNum < m_FrameOffsets.size())
	{
		frame.offset = m_FrameOffsets[frameNum];
	}

	frame.size = m_Size;

	return frame;
}

std::vector<Animation> Animation::LoadAnimations(const std::string& animationFilePath)
{
	std::vector<Animation> animations;

	FileCommandLoader fileLoader;

	Command animationCommand;
	animationCommand.command = "animation";
	animationCommand.parseFunc = [&](ParseFunctionParams params)
	{
		Animation newAnimation;
		newAnimation.SetName(FileCommandLoader::ReadString(params));
		animations.push_back(newAnimation);
	};

	fileLoader.AddCommand(animationCommand);

	Command spriteSheetCommand;
	spriteSheetCommand.command = "sprite_sheet";
	spriteSheetCommand.parseFunc = [&](ParseFunctionParams params)
	{
		animations.back().SetSpriteSheetName(FileCommandLoader::ReadString(params));

	};

	fileLoader.AddCommand(spriteSheetCommand);

	Command sizeCommand;
	sizeCommand.command = "size";
	sizeCommand.parseFunc = [&](ParseFunctionParams params)
	{
		animations.back().SetSize(FileCommandLoader::ReadSize(params));

	};

	fileLoader.AddCommand(sizeCommand);

	Command fpsCommand;
	fpsCommand.command = "fps";
	fpsCommand.parseFunc = [&](ParseFunctionParams params)
	{
		animations.back().SetFPS(FileCommandLoader::ReadInt(params));

	};

	fileLoader.AddCommand(fpsCommand);

	Command framesCommand;
	framesCommand.command = "frame_keys";
	framesCommand.commandType = COMMAND_MULTI_LINE;
	framesCommand.parseFunc = [&](ParseFunctionParams params)
	{
		animations.back().AddFrame(params.line);
	};

	fileLoader.AddCommand(framesCommand);

	Command overlayCommand;
	overlayCommand.command = "overlay";
	overlayCommand.parseFunc = [&](ParseFunctionParams params)
	{
		animations.back().SetOverlay(FileCommandLoader::ReadString(params));
	};

	fileLoader.AddCommand(overlayCommand);

	Command frameColoursCommand;
	frameColoursCommand.command = "frame_colors";
	frameColoursCommand.commandType = COMMAND_MULTI_LINE;
	frameColoursCommand.parseFunc = [&](ParseFunctionParams params)
	{
		animations.back().AddFrameColour(FileCommandLoader::ReadColour(params));
	};

	fileLoader.AddCommand(frameColoursCommand);

	Command overlayFrameColoursCommand;
	overlayFrameColoursCommand.command = "overlay_colors";
	overlayFrameColoursCommand.commandType = COMMAND_MULTI_LINE;
	overlayFrameColoursCommand.parseFunc = [&](ParseFunctionParams params)
	{
		animations.back().AddOverlayFrameColour(FileCommandLoader::ReadColour(params));
	};

	fileLoader.AddCommand(overlayFrameColoursCommand);

	Command frameOffsetsCommand;
	frameOffsetsCommand.command = "frame_offsets";
	frameOffsetsCommand.commandType = COMMAND_MULTI_LINE;
	frameOffsetsCommand.parseFunc = [&](ParseFunctionParams params)
	{
		animations.back().AddOverlayFrameColour(FileCommandLoader::ReadColour(params));
	};

	fileLoader.AddCommand(frameOffsetsCommand);

	assert(fileLoader.LoadFile(animationFilePath));

	return animations;
}
