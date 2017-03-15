#pragma once
#include "ECSInterfaces.h"

struct StaminaComponent : public IComponent, public AutoLister<StaminaComponent>
{
	StaminaComponent(int id, float stamina)
		: IComponent(id)
		, stamina(stamina)
		, boostTime(0)
		, decelerateTime(0)
		, boostActive(false)
	{}

	float stamina;
	float boostTime;
	float decelerateTime;
	bool boostActive;
};

struct AccelerationComponent : public IComponent, public AutoLister<AccelerationComponent>
{
	AccelerationComponent(int id, float acceleration)
		: IComponent(id)
		, acceleration(acceleration)
	{}

	float acceleration;
};

struct ConstMaxAccelerationComponent : public IComponent, public AutoLister<ConstMaxAccelerationComponent>
{
	ConstMaxAccelerationComponent(int id, float acceleration)
		: IComponent(id)
		, MAX_ACCELERATION(acceleration)
	{}

	const float MAX_ACCELERATION;
};

struct ConstBoostedAccelerationComponent : public IComponent, public AutoLister<ConstBoostedAccelerationComponent>
{
	ConstBoostedAccelerationComponent(int id, float acceleration)
		: IComponent(id)
		, BOOSTED_ACCELERATION(acceleration)
	{}

	const float BOOSTED_ACCELERATION;
};

struct VelocityComponent : public IComponent, public AutoLister<VelocityComponent>
{
	VelocityComponent(int id, float velocity)
		: IComponent(id)
		, velocity(velocity)
	{}

	float velocity;
};

struct ConstMaxVelocityComponent : public IComponent, public AutoLister<ConstMaxVelocityComponent>
{
	ConstMaxVelocityComponent(int id, float velocity)
		: IComponent(id)
		, MAX_VELOCITY(velocity)
	{}

	const float MAX_VELOCITY;
};

struct ConstBoostedVelocityComponent : public IComponent, public AutoLister<ConstBoostedVelocityComponent>
{
	ConstBoostedVelocityComponent(int id, float velocity)
		: IComponent(id)
		, BOOSTED_VELOCITY(velocity)
	{}

	const float BOOSTED_VELOCITY;
};
