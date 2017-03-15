#include "stdafx.h"
#include "PhysicsSystem.h"
#include "Logic.h"
#include "Property.h"

void PhysicsSystem::process(float dt)
{
	if (!pausePhysics)
	{
		World().Step(dt, 7, 3);
	}
	


	auto& boxComponents = AutoList::get<Box2DComponent>();
	for (auto& c : boxComponents)
	{
		auto velComp = c->getComponent<VelocityComponent>();
		auto stamComp = c->getComponent<StaminaComponent>();

		if (stamComp && stamComp->stamina <= 0)
		{
			// cannot boost if there is no stamina
			stamComp->boostActive = false;
		}

		if (stamComp && stamComp->boostActive)
		{
			// boost time is the time the boost button on controller is held for
			stamComp->boostTime += dt;
			if (stamComp->stamina > 0)
				stamComp->stamina -= 0.5f;
		}

		if (stamComp && !stamComp->boostActive)
		{
			// lerp deceleration over time 
			DecelerateBoost(dt, c);
		}

		// limiting the max velocity
		b2Vec2 vel = c->body->GetLinearVelocity();
		float32 speed = vel.Length();
		if (velComp != nullptr && speed > velComp->velocity)
			c->body->SetLinearVelocity((velComp->velocity / speed) * vel);
	}

}

Vector2D PhysicsSystem::RayCast(Vector2D start, Vector2D end, float maxLength)
{
	b2RayCastInput input;
	input.p1 = start.toBox2DVector();
	input.p2 = end.toBox2DVector();
	input.maxFraction = 1;

	//check every fixture of every body to find closest
	float closestFraction = 1; //start with end of line as p2
	b2Vec2 intersectionNormal(0, 0);
	for (b2Body* b = World().GetBodyList(); b; b = b->GetNext()) {
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {

			b2RayCastOutput output;
			if (!f->RayCast(&output, input, 0))
				continue;
			if (output.fraction < closestFraction) {
				closestFraction = output.fraction;
				intersectionNormal = output.normal;
			}
		}
	}

	Vector2D intersectionPoint = start + (end - start) * closestFraction;
	return intersectionPoint;
}

std::pair<IEntity*, Vector2D> PhysicsSystem::RayCastToStaticObject(Vector2D start, Vector2D end, float maxLength)
{
	b2RayCastInput input;
	input.p1 = start.toBox2DVector();
	input.p2 = end.toBox2DVector();
	input.maxFraction = 1;

	IEntity* collidedEntity = nullptr;

	//check every fixture of every body to find closest
	float closestFraction = 1; //start with end of line as p2
	b2Vec2 intersectionNormal(0, 0);
	for (b2Body* b = World().GetBodyList(); b; b = b->GetNext()) {
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
			b2RayCastOutput output;
			if (!f->RayCast(&output, input, 0))
				continue;
			if (output.fraction < closestFraction) {
				auto isStatic = ((Box2DComponent*)(b->GetUserData()))->getComponent<StaticBodyComponent>();
				if (!isStatic)
				{
					continue;
				}
				collidedEntity = isStatic->getParent();
				closestFraction = output.fraction;
				intersectionNormal = output.normal;
			}
		}
	}

	Vector2D intersectionPoint = start + (end - start) * closestFraction;
	return{ collidedEntity, intersectionPoint };
}

b2Vec2 & PhysicsSystem::Gravity()
{
	static b2Vec2 gravity(0, 100);
	return gravity;
}

b2World & PhysicsSystem::World()
{
	static b2World world(Gravity());
	world.SetGravity(Gravity());
	return world;
}

void PhysicsSystem::DecelerateBoost(float dt, Box2DComponent* b)
{
	auto stamComp = b->getComponent<StaminaComponent>();
	auto velComp = b->getComponent<VelocityComponent>();
	auto maxVelComp = b->getComponent<ConstMaxVelocityComponent>();
	auto boostedVelComp = b->getComponent<ConstBoostedVelocityComponent>();
	auto accComp = b->getComponent<AccelerationComponent>();
	auto maxAccComp = b->getComponent<ConstMaxAccelerationComponent>();
	auto boostedAccComp = b->getComponent<ConstBoostedAccelerationComponent>();

	stamComp->decelerateTime += dt;

	// while the velocity is greater than original max velocity - reduce the velocity evenly over time (lerp)
	if (velComp->velocity > maxVelComp->MAX_VELOCITY)
	{
		float decelPercentage = stamComp->decelerateTime / stamComp->boostTime;
		float initialValue = boostedVelComp->BOOSTED_VELOCITY - maxVelComp->MAX_VELOCITY;
		float finalValue = 0;
		float decelRate = (initialValue - finalValue) * decelPercentage;
		velComp->velocity = boostedVelComp->BOOSTED_VELOCITY - decelRate;
		accComp->acceleration = maxAccComp->MAX_ACCELERATION;
	}
	else
	{
		velComp->velocity = maxVelComp->MAX_VELOCITY;
		stamComp->decelerateTime = 0;
		stamComp->boostTime = 0;
	}

}

 void PhysicsSystem::setPausePhysics(bool b)
{
	pausePhysics = b;
}
