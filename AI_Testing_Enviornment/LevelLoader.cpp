#include "stdafx.h"
#include "LevelLoader.h"
#include "EntityFactory.h"

std::unordered_map<LEVELS, const char*> LevelLoader::m_paths;
std::vector<IEntity*> LevelLoader::m_entities;

void LevelLoader::RegisterLevels(std::vector<std::pair<LEVELS, const char*>> map)
{
	for (auto element : map)
	{
		m_paths[element.first] = element.second;
	}
}

Vector2D LevelLoader::loadLevel(LEVELS lvl)
{
	destroyLevel();
	FILE* file = new FILE();
	fopen_s(&file, m_paths[lvl], "rb");
	char readBuffer[65536];
	rapidjson::FileReadStream is(file, readBuffer, sizeof(readBuffer));
	rapidjson::Document document;
	document.ParseStream(is);
	fclose(file);

	Vector2D biggest;
	if (document.HasMember("layers"))
	{
		for (const auto& layer : document["layers"].GetArray())
		{
			if (layer["name"] == "Map Layer")
			{
				if (layer.HasMember("objects"))
				{
					for (const auto& itr : layer["objects"].GetArray())
					{
						std::vector<b2Vec2> points;
						if (itr.HasMember("polygon"))
						{
							

							float xPos, yPos;
							xPos = itr["x"].GetFloat() / 50.f;
							yPos = itr["y"].GetFloat() / 50.f;
							for (const auto& it2 : itr["polygon"].GetArray())
							{
								float x, y;
								x = it2["x"].GetFloat() / 50.f;
								y = it2["y"].GetFloat() / 50.f;

								points.push_back(b2Vec2(x + xPos, y + yPos));

								if (x > biggest.w)
								{
									biggest.w = x;
								}
								if (y > biggest.h)
								{
									biggest.h = y;
								}
							}
							EntityFactory::SpawnStaticPoly(points);
						}
						else
						{
							float x, y, w, h;
							x = itr["x"].GetFloat() / 50.f;
							y = itr["y"].GetFloat() / 50.f;
							w = itr["width"].GetFloat() / 50.f;
							h = itr["height"].GetFloat() / 50.f;
							EntityFactory::SpawnStaticBox(x, y, w, h);

							if (x + w > biggest.w)
							{
								biggest.w = x + w;
							}
							if (y + h > biggest.h)
							{
								biggest.h = y + h;
							}
						}
					}
				}
			}
			else if (layer["name"] == "Obstacle Layer")
			{
				if (layer.HasMember("objects"))
				{
					for (const auto& itr : layer["objects"].GetArray())
					{
						float x, y, w, h;
						x = itr["x"].GetFloat() / 50.f;
						y = itr["y"].GetFloat() / 50.f;
						w = itr["width"].GetFloat() / 50.f;
						h = itr["height"].GetFloat() / 50.f;
						EntityFactory::SpawnSoftBox(x, y, w, h);

						if (x + w > biggest.w)
						{
							biggest.w = x + w;
						}
						if (y + h > biggest.h)
						{
							biggest.h = y + h;
						}
					}
				}
			}
			else if (layer["name"] == "Boost Layer")
			{
				if (layer.HasMember("objects"))
				{
					for (const auto& itr : layer["objects"].GetArray())
					{
						float x, y, w, h;
						x = itr["x"].GetFloat() / 50.f;
						y = itr["y"].GetFloat() / 50.f;
						w = itr["width"].GetFloat() / 50.f;
						h = itr["height"].GetFloat() / 50.f;
						EntityFactory::SpawnBoostPad(x, y, w, h);

						if (x + w > biggest.w)
						{
							biggest.w = x + w;
						}
						if (y + h > biggest.h)
						{
							biggest.h = y + h;
						}
					}
				}
			}
			else if (layer["name"] == "Direction Layer")
			{
				if (layer.HasMember("objects"))
				{
					for (const auto& itr : layer["objects"].GetArray())
					{
						float x, y, w, h, priority, directionX, directionY;
						x = itr["x"].GetFloat() / 50.f;
						y = itr["y"].GetFloat() / 50.f;
						w = itr["width"].GetFloat() / 50.f;
						h = itr["height"].GetFloat() / 50.f;
						priority = itr["properties"]["priority"].GetFloat();
						directionX = itr["properties"]["directionX"].GetFloat();
						directionY = itr["properties"]["directionY"].GetFloat();
						EntityFactory::SpawnDirectionVolume(x, y, w, h, priority, Vector2D(directionX, directionY));

						if (x + w > biggest.w)
						{
							biggest.w = x + w;
						}
						if (y + h > biggest.h)
						{
							biggest.h = y + h;
						}
					}
				}
			}
			else if (layer["name"] == "Powerup Layer")
			{
				if (layer.HasMember("objects"))
				{
					for (const auto& itr : layer["objects"].GetArray())
					{
						float x, y, w, h;
						x = itr["x"].GetFloat() / 50.f;
						y = itr["y"].GetFloat() / 50.f;
						w = itr["width"].GetFloat() / 50.f;
						h = itr["height"].GetFloat() / 50.f;
						
						EntityFactory::SpawnPowerUp(x, y, w, h);

						if (x + w > biggest.w)
						{
							biggest.w = x + w;
						}
						if (y + h > biggest.h)
						{
							biggest.h = y + h;
						}
					}
				}

			}
		}
	}
	return biggest;

}

void LevelLoader::destroyLevel()
{
	for (auto i : m_entities)
	{
		delete i;
	}
	m_entities.clear();
}

void LevelLoader::destroyObjects()
{
	for (vector<IEntity*>::iterator it = m_entities.begin(); it != m_entities.end();)
	{
		if (!(*it)->alive)
		{
			delete *it;
			it = m_entities.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void LevelLoader::appendToEntities(IEntity* entity)
{
	m_entities.push_back(entity);
}
