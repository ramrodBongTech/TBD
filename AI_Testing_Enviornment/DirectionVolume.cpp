#include "stdafx.h"
#include "DirectionVolume.h"
#include "Logic.h"

DirectionVolume::DirectionVolume(int id, float x, float y, float w, float h, int priority, Vector2D direction)
	: IEntity(id, {
		new SensorComponent(id, x, y, w, h),
		new DirectionVolumeCollisionResponseComponent(id),
		new PriorityComponent(id, priority),
		new DirectionComponent(id, direction)
	})
{
}
