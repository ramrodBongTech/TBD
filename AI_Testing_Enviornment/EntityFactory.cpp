#include "stdafx.h"
#include "EntityFactory.h"
#include "LevelLoader.h"
#include "RaceManager.h"

void EntityFactory::SpawnPlayer(float x, float y, float w, float h, int controllerId, int colourID, bool isAI)
{
	if (isAI)
	{
		LevelLoader::appendToEntities(new Player(id(), x, y, w, h, colourID));
	}
	/*else
	{
		LevelLoader::appendToEntities(new Player(id(), x, y, w, h, controllerId, colourID, audioMgr));
	}*/
}

void EntityFactory::SpawnStaticBox(float x, float y, float w, float h)
{
	LevelLoader::appendToEntities(new StaticBox(id(), x, y, w, h));
}

void EntityFactory::SpawnSoftBox(float x, float y, float w, float h)
{
	LevelLoader::appendToEntities(new SoftBox(id(), x, y, w, h));
}

bool EntityFactory::SpawnSlowShot(float x, float y, float w, float h, int shooterID)
{
	vector<Player*> players = RaceManager::getInstance()->getPlayers();
	int targetID = 1;

	for (int i = 0; i < players.size(); i++)
	{
		if (i + 1 == players.size())
		{
			return false;
		}
		else if (players[i]->ID == shooterID)
		{
			LevelLoader::appendToEntities(new SlowShot(id(), x, y, w, h, players[i + 1]->ID, shooterID));
			return true;
		}
	}
}

void EntityFactory::SpawnDirectionVolume(float x, float y, float w, float h, int priority, Vector2D direction)
{
	LevelLoader::appendToEntities(new DirectionVolume(id(), x, y, w, h, priority, direction));
}

void EntityFactory::SpawnStaticPoly(std::vector<b2Vec2> points)
{
	LevelLoader::appendToEntities(new StaticPoly(id(), points));
}

void EntityFactory::SpawnWebDrop(float x, float y, float w, float h)
{
	LevelLoader::appendToEntities(new WebDrop(id(), x, y, w, h));
}

void EntityFactory::SpawnBoostPad(float x, float y, float w, float h)
{
	LevelLoader::appendToEntities(new BoostPad(id(), x, y, w, h));
}

void EntityFactory::SpawnPowerUp(float x, float y, float w, float h)
{
	LevelLoader::appendToEntities(new PowerUp(id(), x, y, w, h));
}

void EntityFactory::SpawnHUD(std::string key)
{
	LevelLoader::appendToEntities(new HUD(id(), key));
}