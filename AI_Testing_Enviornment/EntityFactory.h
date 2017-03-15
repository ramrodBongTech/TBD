#pragma once
#include "Entities.h"

class EntityFactory
{
public:
	static void SpawnStaticBox(float x, float y, float w, float h);
	static void SpawnSoftBox(float x, float y, float w, float h);
	static bool SpawnSlowShot(float x, float y, float w, float h, int shooterID);
	static void SpawnPlayer(float x, float y, float w, float h, int controllerId, int colourID = 0, bool isAI = false);
	static void SpawnDirectionVolume(float x, float y, float w, float h, int priority, Vector2D direction);
	static void SpawnStaticPoly(std::vector<b2Vec2> points);
	static void SpawnWebDrop(float x, float y, float w, float h);
	static void SpawnBoostPad(float x, float y, float w, float h);
	static void SpawnPowerUp(float x, float y, float w, float h);
	static void SpawnHUD(std::string key);

private:

};
