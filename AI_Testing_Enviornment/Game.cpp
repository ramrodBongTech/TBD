#include "stdafx.h"
#include "Game.h"
//#include "FLInput\FLInputManager.h"
#include "LevelLoader.h"
#include "EntityFactory.h"
#include "DirectionVolume.h"

bool Game::quit = false;

Game::Game(Vector2D windowSize, Vector2D levelSize, const char* windowName) 
	: m_resourceMgr(ResourceManager::getInstance())
{
	//m_net.setMessageCallback([this](Message m) { this->notifyMessage(m); });
	LevelLoader::RegisterLevels({ //edit enum in LevelLoader.h
		  {LEVELS::LEVEL1, "./assets/levels/map1.json"}
		, { LEVELS::LEVEL2, "./assets/levels/map2.json" }
		, { LEVELS::LEVEL3, "./assets/levels/map3.json" }
		, { LEVELS::LEVEL4, "./assets/levels/map4.json" }
	});

	m_renderer.init(windowSize, windowName, &m_camera);
	m_camera.init(windowSize.w, windowSize.h, m_renderer.getRenderer());

	RaceManager::getInstance()->init(&m_camera);
	srand(time(NULL));

	m_cameraManager = CameraManager();

	//Declare systems
	//auto inputSys = new InputSystem();
	auto renderSys = new RenderSystem();
	auto collisionSystem = new CollisionSystem();
	auto physicsSystem = new PhysicsSystem();
	auto aiSystem = new AISystem();
	auto hookSys = new HookSystem();
	auto scoreSys = new ScoreSystem();
	auto animationSys = new AnimationSystem();
	auto stateSystem = new StateSystem();
	auto powerUpSys = new PowerUpSystem();
	auto swapSys = new SwapSystem();
	auto aiSys = new AISystem();

	//Init systems
	renderSys->init(&m_renderer);
	scoreSys->init(physicsSystem);
	renderSys->setLevel(LEVELS::LEVEL2);
	m_cameraManager.init(&m_camera);
	
	//Push back systems
	m_systems.push_back(collisionSystem);
	m_systems.push_back(hookSys);
	m_systems.push_back(powerUpSys);
	m_systems.push_back(swapSys);
	m_systems.push_back(physicsSystem);
	m_systems.push_back(aiSystem);
	m_systems.push_back(scoreSys);
	m_systems.push_back(animationSys);
	m_systems.push_back(stateSystem);
	m_systems.push_back(aiSys);

	//render system must be added last
	m_systems.push_back(renderSys);
}

void Game::init()
{
	m_resourceMgr->init(&m_renderer);
	m_resourceMgr->loadResources(".//assets//resources.json");
	m_resourceMgr->loadResourceQueue();

	m_cameraManager.SetLevelSize(LevelLoader::loadLevel(LEVELS::LEVEL2));
	m_camera.zoom(-1);

	EntityFactory::SpawnHUD("arrow");

	EntityFactory::SpawnPlayer(50, 12, 1, 1, 0, 0, true);
	EntityFactory::SpawnPlayer(51, 12, 1, 1, 1, 1, true);
	EntityFactory::SpawnPlayer(52, 12, 1, 1, 2, 2, true);
	EntityFactory::SpawnPlayer(53, 12, 1, 1, 3, 3, true);

	for (auto& player : AutoList::get<Player>())
	{
		player->getComponent<InputPauseComponent>()->isPaused = true;
		player->getComponent<InputPauseComponent>()->timeToRun = 2.0f;
	}
}

void Game::loop(float dt)
{
	LevelLoader::destroyObjects();

	for (auto& system : m_systems)
	{
		system->update(dt);
	}

	m_cameraManager.update(dt);
}
