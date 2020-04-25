#pragma once

#include <string>
#include <vector>
#include "Utils/Utils.h"
#include "Math/Vec2D.h"
#include "Colour.h"

struct AnimationFrame
{
	std::string frame = "";
	std::string overlay = "";
	Colour frameColour = Colour::White();
	Colour overlayColour = Colour::White();

	Vec2D size;
	Vec2D offset = Vec2D::Zero;
	int frameColourSet = 0;
};


// This is wrong we should be using a struct here since the entire class is a struct
class Animation
{
public:
	Animation();

	AnimationFrame GetAnimationFrame(uint32_t frameNum) const;

	void AddFrame(const std::string& frame) { m_Frames.push_back(frame); }
	void AddFrameColour(const Colour& colour) { m_FrameColours.push_back(colour); }
	void AddOverlayFrameColour(const Colour& colour) { m_OverlayColours.push_back(colour); }
	void AddOverlayFrameOffset(const Vec2D& offset) { m_FrameOffsets.push_back(offset); }

	void SetSpriteSheetName(const std::string& spriteSheetName) { m_SpriteSheetName = spriteSheetName; }
	const std::string& GetName() const { return m_AnimationName; }

	Vec2D Size() const { return m_Size; }

	inline void SetSize(const Vec2D& size) { m_Size = size; }
	inline void SetFPS(int fps) { m_FPS = fps; }
	inline void SetName(const std::string& name) { m_AnimationName = name; }

	int FPS() const { return m_FPS; }

	void SetOverlay(const std::string& overlayName) { m_Overlay = overlayName; }

	const std::string& OverlayName() const { return m_Overlay; }

	size_t NumFrames() const { return m_Frames.size(); }
	size_t NumFrameColours() const { return m_FrameColours.size(); }
	size_t NumOverlayColours() const { return m_FrameColours.size(); }
	size_t NumFrameOffets() const { return m_FrameColours.size(); }

	static std::vector<Animation> LoadAnimations(const std::string& animationFilePath);

private:
	std::string m_AnimationName;
	std::string m_SpriteSheetName;
	std::string m_Overlay;
	std::vector<std::string> m_Frames;
	std::vector<Colour> m_FrameColours;
	std::vector<Colour> m_OverlayColours;
	std::vector<Vec2D> m_FrameOffsets;

	Vec2D m_Size;
	int m_FPS;
};
