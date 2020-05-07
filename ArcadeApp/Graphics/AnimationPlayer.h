#pragma once

#include <stdint.h>
#include "Animation.h"

class AnimationPlayer
{

public:
	AnimationPlayer();
	bool Init(const std::string& animationsPath);
	bool Play(const std::string& animationName, bool looped);
	void Pause();
	void Stop();
	void Update(uint32_t dt);

	AnimationFrame GetCurrentAnimationFrame() const;

	inline bool IsFinishedPlaying() const { return m_IsFinishedPlaying; }
	inline uint32_t GetCurrentFrameNumber() const { return m_Frame; }

private:
	uint32_t m_Time;
	std::vector<Animation> m_Animations;
	size_t m_CurrentAnimation;
	uint32_t m_Frame;

	bool m_Looped;
	bool m_IsPlaying;
	bool m_IsFinishedPlaying;
};
