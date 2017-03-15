#pragma once

#include "Resource.h"

namespace ANIMATIONS {
	namespace PLAYER {
		namespace BLUE {
			static std::string	IDLERIGHT = "bidleright",
				IDLELEFT = "bidleleft",
				RUNNINGRIGHT = "brunright",
				RUNNINGLEFT = "brunleft",
				ATTACKRIGHT = "battackright",
				ATTACKLEFT = "battackleft",
				FALLRIGHT = "bfallright",
				FALLLEFT = "bfallleft",
				HITRIGHT = "bhitright",
				HITLEFT = "bhitleft",
				LANDRIGHT = "blandingright",
				LANDLEFT = "blandingleft",
				SWINGRIGHT = "bswingingright",
				SWINGLEFT = "bswingingleft";
		}
		namespace RED {
			static std::string	IDLERIGHT = "ridleright",
				IDLELEFT = "ridleleft",
				RUNNINGRIGHT = "rrunright",
				RUNNINGLEFT = "rrunleft",
				ATTACKRIGHT = "rattackright",
				ATTACKLEFT = "rattackleft",
				FALLRIGHT = "rfallright",
				FALLLEFT = "rfallleft",
				HITRIGHT = "rhitright",
				HITLEFT = "rhitleft",
				LANDRIGHT = "rlandingright",
				LANDLEFT = "rlandingleft",
				SWINGRIGHT = "rswingingright",
				SWINGLEFT = "rswingingleft";
		}
		namespace GREEN {
			static std::string	IDLERIGHT = "gidleright",
				IDLELEFT = "gidleleft",
				RUNNINGRIGHT = "grunright",
				RUNNINGLEFT = "grunleft",
				ATTACKRIGHT = "gattackright",
				ATTACKLEFT = "gattackleft",
				FALLRIGHT = "gfallright",
				FALLLEFT = "gfallleft",
				HITRIGHT = "ghitright",
				HITLEFT = "ghitleft",
				LANDRIGHT = "glandingright",
				LANDLEFT = "glandingleft",
				SWINGRIGHT = "gswingingright",
				SWINGLEFT = "gswingingleft";
		}
		namespace YELLOW {
			static std::string	IDLERIGHT = "yidleright",
				IDLELEFT = "yidleleft",
				RUNNINGRIGHT = "yrunright",
				RUNNINGLEFT = "yrunleft",
				ATTACKRIGHT = "yattackright",
				ATTACKLEFT = "yattackleft",
				FALLRIGHT = "yfallright",
				FALLLEFT = "yfallleft",
				HITRIGHT = "yhitright",
				HITLEFT = "yhitleft",
				LANDRIGHT = "ylandingright",
				LANDLEFT = "ylandingleft",
				SWINGRIGHT = "yswingingright",
				SWINGLEFT = "yswingingleft";
		}
	}
}

class Animation
{
public:
	Animation(string _animationName);

	~Animation();

	//void update(float dt);
	void drawAtPosition(Renderer* r, Vector2D pos, Vector2D size, float angle = 0);
	void drawAtHudPosition(Renderer* r, Vector2D pos, Vector2D size, float angle);

	void changeAnimation(string name);
	void resetAnimation();

	void setLooping(bool _isLooping);
	void setFramesPerSecond(float _framesPerSecond);

	bool isAlive();

	void setScale(float s);

	int					m_maxCellHeight;
	int					m_maxCellWidth;
	int					m_frameIndex;
	bool				m_isAlive;
	bool				m_isLooping;
	float				m_animationScale;
	float				m_timeSinceLastFrame;
	float				m_angle;
	float				FPS;
	string				m_selectedAnimation;
	
	vector<Rect>		m_currentFrames;
	SDL_Texture*		m_currentSpriteSheet;
	Rect				m_currentFrame;
};