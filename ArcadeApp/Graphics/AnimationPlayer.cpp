#include "AnimationPlayer.h"

AnimationPlayer::AnimationPlayer():m_Time(0), m_CurrentAnimation(0), m_Frame(0), m_Looped(false), m_IsFinishedPlaying(false), m_IsPlaying(false)
{
}

bool AnimationPlayer::Init(const std::string& animationsPath)
{
	m_Animations = Animation::LoadAnimations(animationsPath);
	return m_Animations.size() > 0;
}

bool AnimationPlayer::Play(const std::string& animationName, bool looped)
{
	bool found = false;

	for (size_t i = 0; i < m_Animations.size(); i++)
	{
		if (m_Animations[i].GetName() == animationName)
		{
			m_CurrentAnimation = i;
			found = true;
			break;
		}
	}

	if (found)
	{
		m_IsPlaying = true;
		m_Frame = 0;
		m_Time = 0;
		m_Looped = looped;
		m_IsFinishedPlaying = false;
	}

	return found;
}

void AnimationPlayer::Pause()
{
	if (m_IsFinishedPlaying)
	{
		m_IsPlaying = !m_IsPlaying;
	}
}

void AnimationPlayer::Stop()
{
	m_IsPlaying = false;
	m_Time = 0;
	m_Frame = 0;
	m_IsFinishedPlaying = true;
}

void AnimationPlayer::Update(uint32_t dt)
{
	if (m_IsPlaying)
	{
		m_Time += dt;

		int fps = m_Animations[m_CurrentAnimation].FPS();

		float millisecondsPerFrame = static_cast<float>(1000) / static_cast<float>(fps);

		if (static_cast<float>(m_Time) > millisecondsPerFrame)
		{
			m_Time -= static_cast<uint32_t>(millisecondsPerFrame);

			if (m_Frame + 1 < m_Animations[m_CurrentAnimation].NumFrames())
			{
				++m_Frame;
			}
			else if ((m_Frame + 1 >= m_Animations[m_CurrentAnimation].NumFrames()) && m_Looped)
			{
				m_Frame = 0;
			}
			else
			{
				m_IsFinishedPlaying = true;
			}
		}
	}
}

AnimationFrame AnimationPlayer::GetCurrentAnimationFrame() const
{
	if (m_Animations.empty())
	{
		return AnimationFrame();
	}
	return m_Animations[m_CurrentAnimation].GetAnimationFrame(GetCurrentFrameNumber());
}
