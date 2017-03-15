#include "stdafx.h"
#include "RaceManager.h"
#include <set>
#include <climits>
#include "DirectionVolume.h"

RaceManager *RaceManager::m_instance = 0;

void RaceManager::init(Camera2D::Camera * camera)
{
	m_camera = camera;
}

std::vector<Player*> RaceManager::Sort()
{
	auto players = AutoList::get<Player>();
	std::set<int> volumeIDs;
	std::vector<Player*> sortedList;
	std::unordered_map<int, std::unordered_map < int, std::pair<int, std::vector<Player* >>> > playerLapsMap;
	std::set<int> playerLaps;

	for (auto player : players)
	{
		auto dirVolume = getComponentById<DirectionVolume>(player->getComponent<RacePositionComponent>()->volumeID);
		if (dirVolume)
		{
			int volume = dirVolume->getComponent<PriorityComponent>()->priority;
			int volumeId = dirVolume->ID;
			int lap = player->getComponent<RacePositionComponent>()->lap;
			playerLapsMap[lap][volume].first = volumeId;
			playerLapsMap[lap][volume].second.push_back(player);
			volumeIDs.insert(volume);
			playerLaps.insert(lap);
		}
	}

	for (auto& lapVolumes : playerLapsMap)
	{
		for (auto& volumePlayers : lapVolumes.second)
		{
			int volumeId = volumePlayers.second.first;
			auto& players = volumePlayers.second.second;
			Vector2D volumeDirection = getComponentById<DirectionVolume>(volumeId)->getComponent<DirectionComponent>()->m_direction;
			std::sort(players.begin(), players.end(), [&](Player* p1, Player* p2) 
			{
				return	Vector2D::DotProduct(volumeDirection, p1->getComponent<Box2DComponent>()->body->GetPosition()) <
					Vector2D::DotProduct(volumeDirection, p2->getComponent<Box2DComponent>()->body->GetPosition());
			});
		}
	}

	for (int lap : playerLaps)
	{
		for (int volume : volumeIDs)
		{
			for (auto player : playerLapsMap[lap][volume].second)
			{
				sortedList.push_back(player);
			}
		}
	}

	////players are not in direction volumes when spawned
	//assert(sortedList.size() != 0);

	return sortedList;
}

std::vector<Player*> RaceManager::getPlayers()
{
	return Sort();
}

std::vector<Player*> RaceManager::getOnScreenPlayers()
{
	CustomRect cameraBounds = m_camera->getBounds();
	std::vector<Player*> returnVector;
	SDL_Point playerPos;

	m_players = AutoList::get<Player>();

	for (std::vector<Player*>::iterator it = m_players.begin(); it != m_players.end(); ++it)
	{
		auto player = (*it)->getComponent<Box2DComponent>();
		if (player)
		{
			playerPos.x = (*it)->getComponent<Box2DComponent>()->body->GetPosition().x;
			playerPos.y = (*it)->getComponent<Box2DComponent>()->body->GetPosition().y;
		}
		else
		{
			playerPos.x = INT_MAX;
			playerPos.y = INT_MAX;
		}

		auto bounds = SDL_Rect();
		bounds.x = cameraBounds.x;
		bounds.y = cameraBounds.y;
		bounds.w = cameraBounds.w;
		bounds.h = cameraBounds.h;
		
		if (SDL_PointInRect(&playerPos, &bounds) && getComponentById<ScoreComponent>(player->ID)->alive)
		{
			returnVector.push_back(*it);
		}
	}
	return returnVector;
}

Player* RaceManager::getLeader()
{
	auto sortedPlayers = Sort();
	if (sortedPlayers.size() != 0)
	{
		return sortedPlayers.back();
	}
	 return nullptr;
}