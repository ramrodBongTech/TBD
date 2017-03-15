#pragma once
#include "ECSInterfaces.h"
#include "Vector2D.h"
#include "Drawables.h"
#include "PhysicsSystem.h"

struct HookComponent : public IComponent, public AutoLister<HookComponent>
{
	HookComponent(int id, Vector2D start, Vector2D end, b2Body* bodyToAttach);

	void decreaseTetherLength(float dt);

	void increaseTetherLength(float dt);

	virtual ~HookComponent();

	b2DistanceJoint* joint;
	Box2DComponent* pivot;
	LineComponent* line;
	float tetherLength;
	bool alive;
};

struct AbilityComponent : public IComponent, public AutoLister<AbilityComponent>
{
	enum ABILITIES {
		NONE,
		WEB_DROP,
		SLOW_SHOT,
		SWAP_SHOT
	};

	AbilityComponent(int id)
		: IComponent(id)
		, canAnimate(false)
	{
	}

	ABILITIES ability = NONE;
	bool canAnimate;
};

struct SwapComponent : public IComponent, public AutoLister<SwapComponent>
{
	SwapComponent(int id, Vector2D start, Vector2D end, b2Body* myBody, IEntity* targetBody);

	virtual ~SwapComponent();

	IEntity* target;
	b2RopeJoint* joint;
	LineComponent* line;
	float tetherLength;
	bool isShot;
	float ttl;
};
