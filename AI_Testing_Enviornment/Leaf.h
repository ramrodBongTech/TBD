#pragma once
#include "Node.h"
#include "Interactables.h"
#include "DirectionVolume.h"
#include "Property.h"

class Leaf : public Node
{
public:
	virtual ~Leaf() {}
};

class UseAbility : public Leaf
{
public:
	UseAbility();
	~UseAbility();

	Status Update(IEntity* p, float dt, bool isHooked);
};

class MoveInDirectionOfVolume : public Leaf
{
public:
	MoveInDirectionOfVolume();
	~MoveInDirectionOfVolume();

	Status Update(IEntity* p, float dt, bool isHooked);
};

class UseHook : public Leaf
{
public:
	UseHook() {}
	~UseHook() {}

	Status Update(IEntity* p, float dt, bool isHooked)
	{
		if (!isHooked)
		{
			auto b = p->getComponent<Box2DComponent>();
			auto h = p->getComponent<HookComponent>();
			if (!h && b)
			{
				auto intersection = PhysicsSystem::RayCastToStaticObject(b->body->GetPosition(), Vector2D(b->body->GetPosition()) + Vector2D(/*xRay*/0, -1000));
				if (intersection.first)
				{
					auto isStatic = intersection.first->getComponent<StaticBodyComponent>();
					float distance = Vector2D::Distance(Vector2D(b->body->GetPosition()), intersection.second);
					if (distance > 8 && isStatic)
					{
						p->AddComponent(new HookComponent(p->ID, b->body->GetPosition(), intersection.second, b->body));
						return Status::Success;
					}
				}
			}
		}
		return Status::Failure;
	}
};

class RaiseHook : public Leaf
{
public:
	RaiseHook() {}
	~RaiseHook() {}

	Status Update(IEntity* p, float dt, bool isHooked)
	{
		if (!isHooked)
		{
			auto h = p->getComponent<HookComponent>();
			if (h)
			{
				if (h->tetherLength < 8)
				{
					h->getParent()->deleteComponent<HookComponent>();
					return Status::Failure;
				}
				else
				{
					h->decreaseTetherLength(dt);
					return Status::Success;
				}
			}
		}
		return Status::Failure;
	}
};

class UseStamina : public Leaf
{
public:
	UseStamina() {}
	~UseStamina() {}

	Status Update(IEntity* p, float dt, bool isHooked)
	{
		if (!isHooked)
		{
			auto s = p->getComponent<StaminaComponent>();
			auto a = p->getComponent<AccelerationComponent>();
			auto v = p->getComponent<VelocityComponent>();
			auto bv = p->getComponent<ConstBoostedVelocityComponent>();
			auto ba = p->getComponent<ConstBoostedAccelerationComponent>();

			if (s && a && v && bv && ba)
			{
				if (s->stamina > 0)
				{
					s->boostActive = true;
					s->stamina--;
					a->acceleration = ba->BOOSTED_ACCELERATION;
					v->velocity = bv->BOOSTED_VELOCITY;
					return Status::Success;
				}
				else
				{
					s->boostActive = false;
					return Status::Failure;
				}
			}
		}
		return Status::Failure; 
	}
};

class CheckVelocity : public Leaf
{
public:
	CheckVelocity() {}
	~CheckVelocity() {}

	Status Update(IEntity* p, float dt, bool isHooked)
	{
		if (!isHooked)
		{
			auto b = p->getComponent<Box2DComponent>();
			auto s = p->getComponent<StaminaComponent>();
			if (b && s)
			{
				auto vel = b->body->GetLinearVelocity();
				if (vel.x < 15)
				{
					return Status::Success;
				}
				else
				{
					s->boostActive = false;
					return Status::Failure;
				}
			}
		}
		return Status::Failure;
	}
};

class CheckHooked : public Leaf
{
public:
	CheckHooked() {}
	~CheckHooked() {}

	Status Update(IEntity* p, float dt, bool isHooked)
	{
		if (!isHooked)
		{
			auto h = p->getComponent<HookComponent>();
			if (h)
			{
				return Status::Success;
			}
		}
		return Status::Failure;
	}
};