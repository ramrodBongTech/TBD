#pragma once
#include "PowerUp.h"
#include "Logic.h"

class PowerUpSystem : public ISystem
{
public:
	PowerUpSystem()
	{}

	void process(float dt) override
	{
		for (auto& component : AutoList::get<PowerUp>())
		{
			auto r = component->getComponent<PowerUpRespawnComponent>();
			if (r && r->isDead)
			{
				r->getComponent<Box2DComponent>()->body->SetTransform(b2Vec2(999,999), r->getComponent<Box2DComponent>()->body->GetAngle());
				r->tts += dt;
				if (r->tts >= 10)
				{
					r->ReSpawn();
				}
			}
		}
	}
};