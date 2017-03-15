#include "stdafx.h"
#include "Interactables.h"
#include "Dimensional.h"

HookComponent::HookComponent(int id, Vector2D start, Vector2D end, b2Body* bodyToAttach)
	: IComponent(id)
	, pivot( new StaticBodyComponent(-1, end.x, end.y, 0, 0, true))
	, tetherLength(Vector2D::Distance(start, end) * 0.9f)
	, line(new LineComponent(-1, start, end))
	, alive(true)
{
	b2DistanceJointDef jointDef;
	jointDef.bodyA = bodyToAttach;
	jointDef.bodyB = pivot->body;
	jointDef.collideConnected = false;
	jointDef.length = tetherLength;
	jointDef.dampingRatio = 0.5;
	jointDef.frequencyHz = 60;

	joint = (b2DistanceJoint*)PhysicsSystem::World().CreateJoint(&jointDef);
}

void HookComponent::decreaseTetherLength(float dt)
{
	tetherLength -= 30 * dt;
	joint->SetLength(tetherLength);
}

void HookComponent::increaseTetherLength(float dt)
{
	tetherLength += 2 * dt;
	joint->SetLength(tetherLength);
}

HookComponent::~HookComponent()
{
	PhysicsSystem::World().DestroyJoint(joint);
	delete pivot;
	delete line;
}

SwapComponent::SwapComponent(int id, Vector2D start, Vector2D end, b2Body* myBody, IEntity* targetBody)
	: IComponent(id)
	, target(targetBody)
	, line(new LineComponent(-1, start, end))
	, tetherLength(Vector2D::Distance(start, end))
	, isShot(true)
	, ttl(0)
{
	b2RopeJointDef jointDef;
	jointDef.bodyA = myBody;
	jointDef.bodyB = targetBody->getComponent<Box2DComponent>()->body;
	jointDef.collideConnected = false;
	jointDef.maxLength = tetherLength;

	joint = (b2RopeJoint*)PhysicsSystem::World().CreateJoint(&jointDef);
}

SwapComponent::~SwapComponent()
{
	PhysicsSystem::World().DestroyJoint(joint);
	delete line;
}