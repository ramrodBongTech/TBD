#pragma once
#include "Components.h"

class BoostPad : public IEntity, public AutoLister<BoostPad>
{
public:
	BoostPad(int id, int x, int y, int w, int h) : IEntity(id,
	{
		new SensorComponent(id, x, y, w, h),
		new BoostPadResponseComponent(id)
	})
	{};


private:

};