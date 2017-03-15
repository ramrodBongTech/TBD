#pragma once
#include "Player.h"
#include "camera\Camera2D.h"
#include <numeric>

class RaceManager
{
public:
	static RaceManager *m_instance;

	void init(Camera2D::Camera * camera);

	std::vector<Player*> Sort();
	std::vector<Player*> getPlayers();
	std::vector<Player*> getOnScreenPlayers();
	Player* getLeader();

	static RaceManager *getInstance()
	{
		if (!m_instance)
		{
			m_instance = new RaceManager();
		}
		return m_instance;
	}

private:
	std::vector<Player*> m_players;
	Camera2D::Camera * m_camera;
};

