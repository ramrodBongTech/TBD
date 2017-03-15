#pragma once
#include "ECSInterfaces.h"
#include "RaceManager.h"
#include <algorithm>
#include <set>

class ScoreSystem : public ISystem
{
public:
	ScoreSystem()
	{
	}

	void init(PhysicsSystem* p)
	{
		physicsSys = p;
	}

	void process(float dt) override
	{
		std::vector<Player*> onScreen = RaceManager::getInstance()->getOnScreenPlayers();
		if (onScreen.size() == 1 && AutoList::get<Player>().size() > 1)
		{
			Player* winner = onScreen.at(0);
			getComponentById<ScoreComponent>(winner->ID)->rounds++;
			//new round
			for (auto& player : AutoList::get<Player>())
			{
				player->getComponent<DynamicBodyComponent>()->body->SetTransform(winner->getComponent<DynamicBodyComponent>()->body->GetPosition(), 0);
				player->getComponent<RacePositionComponent>()->lap = winner->getComponent<RacePositionComponent>()->lap;
				player->getComponent<RacePositionComponent>()->volumeID = winner->getComponent<RacePositionComponent>()->volumeID;
				player->getComponent<InputPauseComponent>()->isPaused = true;
				player->getComponent<InputPauseComponent>()->timeToRun = 4.0f;

				if (player->getComponent<HookComponent>())
				{
					player->deleteComponent<HookComponent>();
				}

				auto& Countdowns = AutoList::get<CountdownComponent>();
				for (auto& Countdown : Countdowns)
				{
					Countdown->timeToDisplay = 4.0f;
					Countdown->animation.resetAnimation();
				}

				player->getComponent<ScoreComponent>()->alive = true;
				player->getComponent<DynamicBodyComponent>()->fixture->SetSensor(false);
			}
			physicsSys->setPausePhysics(true);
			//Step twice so that players aren't all in exact same position
			PhysicsSystem::World().Step(dt * 2, 7, 3);

			if (getComponentById<ScoreComponent>(winner->ID)->rounds >= 3)
			{
				//DO WIN 
				//Sean's scene code here
				reset();
			}
		}
		else
		{
			if (AutoList::get<Player>().size() > 0 && onScreen.size() > 0)
			{
				std::vector<Player*> losers = difference(AutoList::get<Player>(), onScreen);
				for (std::vector<Player*>::iterator it = losers.begin(); it != losers.end(); ++it)
				{
					if ((*it)->getComponent<ScoreComponent>()->alive && RaceManager::getInstance()->getPlayers().front() == (*it))
					{
						(*it)->getComponent<ScoreComponent>()->alive = false;
						(*it)->getComponent<DynamicBodyComponent>()->fixture->SetSensor(true);
					}
				}
			}
		}

		if (onScreen.size() > 0)
		{
			auto comp = onScreen.at(0)->getComponent<InputPauseComponent>();
			if (comp && comp->timeToRun <= 0)
			{
				physicsSys->setPausePhysics(false);
			}
		}
		else
		{
			physicsSys->setPausePhysics(false);
		}


	}

	void reset()
	{
		for (auto& component : AutoList::get<ScoreComponent>())
		{
			component->rounds = 0;
			component->alive = true;
		}
	}

	std::vector<Player*> difference(std::vector<Player*> v1, std::vector<Player*> v2)
	{
		std::set<Player*> s1(v1.begin(), v1.end());
		std::set<Player*> s2(v2.begin(), v2.end());

		std::vector<Player*> result;
		std::vector<Player*>::iterator it;

		std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), std::back_inserter(result));

		return result;
	}

private:
	PhysicsSystem* physicsSys;
};
