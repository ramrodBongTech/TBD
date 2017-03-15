#pragma once
#include "Components.h"

class PowerUp : public IEntity, public AutoLister<PowerUp>
{
public:
	PowerUp(int id, float x, float y, float w, float h)
		: IEntity(id,
		{
			new SensorComponent(id, x, y, w, h),
			new PowerUpResponseComponent(id),
			new PowerUpRespawnComponent(id, x , y)
		})
	{}
};
