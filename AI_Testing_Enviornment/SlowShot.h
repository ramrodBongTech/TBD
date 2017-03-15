#pragma once
#include "Components.h"

class SlowShot : public IEntity, public AutoLister<SlowShot>
{
public:
	SlowShot(int id, float x, float y, float w, float h, int targetID, int shooterID)
		: IEntity(id,
		{
			new SlowShotComponent(id, x, y, w, h, false),
			new SeekAIComponent(id, targetID, shooterID),
			new SlowShotResponseComponent(id)
		})
	{}

	virtual ~SlowShot() {}

};
