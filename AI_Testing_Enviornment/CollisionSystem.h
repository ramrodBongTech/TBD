#pragma once
#include "ECSInterfaces.h"
#include "Dimensional.h"
#include "Logic.h"

class CollisionSystem : public ISystem, public b2ContactListener
{
public:
	void process(float dt) override
	{

	}

	CollisionSystem()
	{
		PhysicsSystem::World().SetContactListener(this);
	}

	void BeginContact(b2Contact * contact)
	{
		void * bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
		void * bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

		if (bodyUserDataA && bodyUserDataB)
		{
			auto boxComponentA = static_cast<Box2DComponent*>(bodyUserDataA);
			auto collisionResponseA = boxComponentA->getComponent<ICollisionResponseComponent>();

			auto boxComponentB = static_cast<Box2DComponent*>(bodyUserDataB);
			auto collisionResponseB = boxComponentB->getComponent<ICollisionResponseComponent>();

			IEntity* parent = nullptr;

			if (collisionResponseA)
			{
				parent = boxComponentB->getParent();
				if (parent)
				{
					collisionResponseA->beginContact(parent);
				}
			}
			if (collisionResponseB)
			{
				parent = boxComponentA->getParent();
				if (parent)
				{
					collisionResponseB->beginContact(parent);
				}
			}
		}
	}

	void EndContact(b2Contact * contact)
	{
		void * bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
		void * bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

		if (bodyUserDataA && bodyUserDataB)
		{
			auto boxComponentA = static_cast<Box2DComponent*>(bodyUserDataA);
			auto collisionResponseA = boxComponentA->getComponent<ICollisionResponseComponent>();

			auto boxComponentB = static_cast<Box2DComponent*>(bodyUserDataB);
			auto collisionResponseB = boxComponentB->getComponent<ICollisionResponseComponent>();

			IEntity* parent = nullptr;

			if (collisionResponseA)
			{
				parent = boxComponentB->getParent();
				if (parent)
				{
					collisionResponseA->endContact(parent);
				}
			}
			if (collisionResponseB)
			{
				parent = boxComponentA->getParent();
				if (parent)
				{
					collisionResponseB->endContact(parent);
				}
			}
		}
	}
private:

};
