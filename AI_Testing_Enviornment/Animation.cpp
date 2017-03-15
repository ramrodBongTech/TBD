#include "stdafx.h"
#include "Animation.h"



Animation::Animation(string _animationName) 
	: m_maxCellHeight(0)
	, m_maxCellWidth(0)
	, m_isAlive(true)
	, m_isLooping(true)
	, m_animationScale(1.2)
	, m_timeSinceLastFrame(0)
	, m_angle(0)
	, FPS(1/60)
	, m_selectedAnimation(_animationName)
	, m_currentSpriteSheet(nullptr)
	, m_currentFrame(Rect())
{

	auto& data = ResourceManager::getInstance()->getAnimationByKey(_animationName);
	m_currentSpriteSheet = data.first;
	m_currentFrames = data.second;
	m_currentFrame = m_currentFrames[0];

	for (int i = 0; i < m_currentFrames.size(); i++)
	{
		if (m_currentFrames[i].size.w > m_maxCellWidth)
		{
			m_maxCellWidth = m_currentFrames[i].size.w;
		}
		if (m_currentFrames[i].size.h > m_maxCellHeight)
		{
			m_maxCellHeight = m_currentFrames[i].size.h;
		}
	}
}

Animation::~Animation() {}

void Animation::changeAnimation(string _animationName)
{
	if (_animationName != m_selectedAnimation)
	{
		m_selectedAnimation = _animationName;
		resetAnimation();
	}
}

void Animation::resetAnimation()
{
	m_frameIndex = 0;
	auto& data = ResourceManager::getInstance()->getAnimationByKey(m_selectedAnimation);
	m_currentSpriteSheet = data.first;
	m_currentFrames = data.second;
	m_currentFrame = m_currentFrames[0];
}

void Animation::setLooping(bool _isLooping)
{
	m_isLooping = _isLooping;
}

void Animation::setFramesPerSecond(float _framesPerSecond)
{
	FPS = 1 / _framesPerSecond;
}

bool Animation::isAlive()
{
	return m_isAlive;
}

void Animation::setScale(float s)
{
	m_animationScale = s;
}

void Animation::drawAtPosition(Renderer* r, Vector2D pos, Vector2D size, float angle)
{
	if (m_isAlive)
	{
		r->drawTextureWithAngle(m_currentSpriteSheet, m_currentFrame, Rect(pos, size), (angle * (180 / 3.14)));
	}
}

void Animation::drawAtHudPosition(Renderer* r, Vector2D pos, Vector2D size, float angle)
{
	if (m_isAlive)
	{
		r->drawTextureWithAngleHud(m_currentSpriteSheet, m_currentFrame, Rect(pos, size), (angle * (180 / 3.14)));
	}
}