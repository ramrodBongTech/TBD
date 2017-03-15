#pragma once
#include "ECSInterfaces.h"
#include "Logic.h"

class AISystem : public ISystem
{
public:
	AISystem()
	{}

	void process(float dt) override
	{
		auto& ais = AutoList::get<AIComponent>();
		for (auto& component : ais)
		{
			component->think(dt);
		}
	}
};