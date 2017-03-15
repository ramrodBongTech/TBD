#pragma once
#include "Components.h"

class HUD : public IEntity, public AutoLister<HUD>
{
public:
	HUD(int id, std::string key) : IEntity(id,
	{
		new DirectionArrowComponent(id, key),
		new CountdownComponent(id, "countdown")
	})
	{};


private:

};