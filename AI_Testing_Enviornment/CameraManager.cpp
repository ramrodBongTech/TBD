#include "stdafx.h"
#include "CameraManager.h"

CameraManager::CameraManager() : m_levelSize(0,0)
{
}

void CameraManager::update(float dt)
{

	m_camera->update(dt);
	//Track leader
	Player *leader = RaceManager::getInstance()->getLeader();

	int volumeID;

	if (leader)
	{
		auto raceComponent = leader->getComponent<RacePositionComponent>();

		if (raceComponent)
		{
			volumeID = raceComponent->volumeID;
		}

		if (RaceManager::getInstance()->getLeader()->getComponent<Box2DComponent>())
		{
			moveTo(RaceManager::getInstance()->getLeader()->getComponent<Box2DComponent>()->body->GetPosition(), dt);
		}
	}

	//Set Zoom
	std::vector<Camera2D::Point> pts;
	std::vector<Player*> players = RaceManager::getInstance()->getPlayers();

	for (int i = 0; i < players.size(); i++)
	{
		Camera2D::Point p;

		p.x = players.at(i)->getComponent<DynamicBodyComponent>()->body->GetPosition().x;
		p.y = players.at(i)->getComponent<DynamicBodyComponent>()->body->GetPosition().y;

		pts.push_back(p);
	}

	m_camera->zoomToFit(pts, false);



}

void CameraManager::init(Camera2D::Camera * cam)
{
	m_camera = cam;
}

void CameraManager::moveTo(Vector2D destination, float dt)
{
	Vector2D position = Vector2D(m_camera->getCentre().x, m_camera->getCentre().y);
	Vector2D difference = destination - position;
	Vector2D directionToPan = difference.Normalize();
	float distance = difference.Distance(position, difference);

	//if (difference.Magnitude() > 1)
	//{
	//	Vector2D result = position + (directionToPan * distance * dt / 2.f);

	//	m_camera->setCentre(result.x, result.y);

	//	auto cameraBounds = m_camera->getBounds();

	//	int halfCameraWidth = cameraBounds.w / 2.0f;
	//	int halfCameraHeight = cameraBounds.h / 2.0f;

	//	auto centre = m_camera->getCentre();
	//	if (cameraBounds.x < 0)
	//	{
	//		m_camera->setCentre(halfCameraWidth, centre.y);
	//		centre = m_camera->getCentre();
	//	}

	//	if (cameraBounds.y < 0)
	//	{
	//		m_camera->setCentre(centre.x, halfCameraHeight);
	//		centre = m_camera->getCentre();
	//	}

	//	if (cameraBounds.x + cameraBounds.w > m_levelSize.w)
	//	{
	//		m_camera->setCentre(m_levelSize.w - halfCameraWidth, centre.y);
	//		centre = m_camera->getCentre();
	//	}

	//	if (cameraBounds.y + cameraBounds.h >  m_levelSize.h)
	//	{
	//		m_camera->setCentre(centre.x, m_levelSize.h - halfCameraHeight);
	//		centre = m_camera->getCentre();
	//	}
	//}
	//else
	//{
		m_camera->setCentre(destination.x, destination.y);

		auto cameraBounds = m_camera->getBounds();

		int halfCameraWidth = cameraBounds.w / 2.0f;
		int halfCameraHeight = cameraBounds.h / 2.0f;

		auto centre = m_camera->getCentre();
		if (cameraBounds.x < 0)
		{
			m_camera->setCentre(halfCameraWidth, centre.y);
			centre = m_camera->getCentre();
		}

		if (cameraBounds.y < 0)
		{
			m_camera->setCentre(centre.x, halfCameraHeight);
			centre = m_camera->getCentre();
		}

		if (cameraBounds.x + cameraBounds.w > m_levelSize.w)
		{
			m_camera->setCentre(m_levelSize.w - halfCameraWidth, centre.y);
			centre = m_camera->getCentre();
		}

		if (cameraBounds.y + cameraBounds.h >  m_levelSize.h)
		{
			m_camera->setCentre(centre.x, m_levelSize.h - halfCameraHeight);
			centre = m_camera->getCentre();
		}
//	}
}

void CameraManager::SetLevelSize(Vector2D size)
{
	m_levelSize = size;
}