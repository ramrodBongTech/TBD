#pragma once
#include "ECSInterfaces.h"
#include "Interactables.h"
#include "Dimensional.h"

class HookSystem : public ISystem
{
public:
	void process(float dt) override
	{
		auto components = AutoList::get<HookComponent>();
		for (auto hook : components)
		{
			auto collider = hook->getComponent<Box2DComponent>();
			if (collider)
			{
				hook->line->start = collider->body->GetPosition();
			}
		}
	}
private:

};
