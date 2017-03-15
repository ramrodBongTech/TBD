#pragma once
#include "Player.h"
#include "camera\Camera2D.h"
#include "RaceManager.h"

class CameraManager
{
public:
	CameraManager();

	void SetLevelSize(Vector2D size);
	void init(Camera2D::Camera * cam);
	void moveTo(Vector2D destination, float dt);

	void update(float dt);

private:
	Camera2D::Camera * m_camera;
	Vector2D m_levelSize;
};

