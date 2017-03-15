#pragma once
#include "Components.h"
#include "Logic.h"

class ICollisionResponseComponent;
struct PlayerStaticObjectResponseComponent : public ICollisionResponseComponent
{
	PlayerStaticObjectResponseComponent(int id)
		: ICollisionResponseComponent(id)
	{}

	void endContact(IEntity * e)
	{

	};

	void beginContact(IEntity * e)
	{
		if (e)
		{
			auto isStaticObject = e->getComponent<StaticBodyComponent>();
			if (isStaticObject)
			{
				auto hook = getComponent<HookComponent>();
				if (hook)
				{
					hook->alive = false;
				}
				return;
			}

			auto collisionBody = e->getComponent<SoftObstacleResponseComponent>(); // collision with sensor component
			if (collisionBody)
			{
				auto b = getComponent<DynamicBodyComponent>();
				if (b)
				{
					b->body->SetLinearVelocity(b2Vec2(0, 0));
				}
			}

			auto ss = e->getComponent<SlowShotComponent>();
			if (ss)
			{
				auto b = getComponent<DynamicBodyComponent>();
				if (b)
				{
					b->body->SetLinearVelocity(b2Vec2(0, 0));
				}
			}

			auto puResponse = e->getComponent<PowerUpResponseComponent>();
			if (puResponse)
			{
				auto puRespawn = e->getComponent<PowerUpRespawnComponent>();
				if (puRespawn)
				{
					puRespawn->Die();

					auto a = getComponent<AbilityComponent>();
					if (a)
					{
						if (a->ability == a->NONE)
						{
							getComponent<HudComponent>()->spinTime = 1;
							switch (rand() % 3)
							{
							case 0:
								
								a->ability = a->WEB_DROP;
								break;
							case 1:
								a->ability = a->SLOW_SHOT;
								break;
							case 2:
								a->ability = a->SWAP_SHOT;
								break;
							}
							a->canAnimate = true;
						}
					}
				}
			}
		}
	}
};

class Player : public IEntity, public AutoLister<Player>
{
public:
	//Player(int id, float x, float y, float w, float h, int controllerId, int colourID, AudioManager* audioMgr)
	//	: IEntity(id,
	//	{
	//		new DynamicBodyComponent(id, x, y, w, h, false),
	//		new StaminaComponent(id, 100),
	//		new AccelerationComponent(id, 20),
	//		new ConstMaxAccelerationComponent(id, 20),
	//		new ConstBoostedAccelerationComponent(id, 150),
	//		new VelocityComponent(id, 50),
	//		new ConstMaxVelocityComponent(id, 50),
	//		new ConstBoostedVelocityComponent(id, 80),
	//		new PlayerControllerComponent(id, controllerId, audioMgr),
	//		new RacePositionComponent(id),
	//		new PlayerStaticObjectResponseComponent(id, audioMgr),
	//		new InputPauseComponent(id, false),
	//		new ScoreComponent(id),
	//		new AnimationComponent(id, "bidleright", colourID),
	//		new HudComponent(id, "abilityIcon"),
	//		new StateComponent(id),
	//		new AbilityComponent(id)
	//	})
	//{
	//}

	Player(int id, float x, float y, float w, float h, int colourID)
		: IEntity(id,
		{
			new DynamicBodyComponent(id, x, y, w, h, false),
			new StaminaComponent(id, 100),
			new AccelerationComponent(id, 20),
			new ConstMaxAccelerationComponent(id, 20),
			new ConstBoostedAccelerationComponent(id, 150),
			new VelocityComponent(id, 50),
			new ConstMaxVelocityComponent(id, 50),
			new ConstBoostedVelocityComponent(id, 80),
			new PlayerAIComponent(id),
			new RacePositionComponent(id),
			new PlayerStaticObjectResponseComponent(id),
			new AnimationComponent(id, "bidleright", colourID),
			new StateComponent(id),
			new HudComponent(id, "abilityIcon"),
			new AbilityComponent(id),
			new InputPauseComponent(id, false),
			new ScoreComponent(id)
		})
	{
	}
private:

};