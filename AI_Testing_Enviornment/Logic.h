#pragma once
#include <functional>
#include "ECSInterfaces.h"
#include "Dimensional.h"
#include "RacePosition.h"
#include "BehaviourTree.h"

struct ICollisionResponseComponent : public AutoLister<ICollisionResponseComponent>, public IComponent
{
	ICollisionResponseComponent(int id)
		: IComponent(id)
	{}

	virtual ~ICollisionResponseComponent() {};

	virtual void endContact(IEntity* e)
	{

	};


	virtual void beginContact(IEntity* e)
	{

	};
};

struct SoftObstacleResponseComponent : public ICollisionResponseComponent, public AutoLister<SoftObstacleResponseComponent>
{
	SoftObstacleResponseComponent(int id)
		:	ICollisionResponseComponent(id)
	{

	}

	void endContact(IEntity * e)
	{
	
	};

	void beginContact(IEntity * e)
	{
		auto collisionBody = e->getComponent <DynamicBodyComponent> ();
		
		if (collisionBody)
		{
			auto b = getComponent<KinematicBodyComponent>();
			if (b)
				b->body->SetLinearVelocity(b2Vec2(0, 100000));
		}
	};
};

struct AIComponent : public IComponent, public AutoLister<AIComponent>
{
	AIComponent(int id) 
		:	IComponent(id)
	{
	}

	virtual void think(float dt) {}
};

struct InputPauseComponent : public AutoLister<InputPauseComponent>, public IComponent
{
	float timeToRun;
	//float timeToPause;
	bool isPaused;

	InputPauseComponent(int id, bool p)
		: IComponent(id),
		timeToRun(0),
		//timeToPause(0),
		isPaused(p)
	{

	}
};

struct ScoreComponent : public AutoLister<ScoreComponent>, public IComponent
{
	int rounds;
	bool alive;
	int colourID;

	ScoreComponent(int id, int colour = 0)
		: IComponent(id),
		rounds(0),
		alive(true),
		colourID(colour)
	{

	}
};

struct SeekAIComponent : public AIComponent, public AutoLister<SeekAIComponent>
{
	SeekAIComponent(int id, int target_id, int shooter_id)
		: AIComponent(id)
		, target(getComponentById<Box2DComponent>(target_id))
		, shooterID(shooter_id)
	{
		assert(target != nullptr);
	}

	void think(float dt)
	{
		if (target != nullptr)
		{
			auto object = getComponent<Box2DComponent>();

			b2Vec2 pos = object->body->GetPosition();
			b2Vec2 targetPos = target->body->GetPosition();

			auto direction = target->body->GetPosition() - object->body->GetPosition();
			float length = sqrt((direction.x * direction.x) + (direction.y * direction.y));
			direction = b2Vec2(direction.x / length, direction.y / length);
			b2Vec2 velocity = b2Vec2(direction.x * 1000, direction.y * 1000);
			object->body->SetLinearVelocity(velocity);

			// change orienation of sprite if needed
			//if (sqrt((direction.x * direction.x) + (direction.y * direction.y)) >= 0)
			//{
			//	float orientation = atan2(direction.x, -direction.y) * (180 / 3.141592654);
			//	// set sprite rotation
			//}
		}
	}

	Box2DComponent* target;
	int shooterID;
};

struct PlayerAIComponent : public AIComponent, public AutoLister<PlayerAIComponent>
{
	PlayerAIComponent(int id)
		: AIComponent(id)
		, bt(BehaviourTree())
		, isHooked(false)
	{
		Selector* root = new Selector();
		root->Initialize();
		root->AddChild(new UseAbility());

		Sequence* checkHook = new Sequence();
		checkHook->Initialize();
		checkHook->AddChild(new CheckHooked());
		checkHook->AddChild(new RaiseHook());
		root->AddChild(checkHook);

		Selector* moveSelector = new Selector();
		moveSelector->Initialize();

		Sequence* staminaSequence = new Sequence();
		staminaSequence->Initialize();
		staminaSequence->AddChild(new CheckVelocity());
		staminaSequence->AddChild(new UseStamina());

		Failer* staminaFailer = new Failer();
		staminaFailer->SetChild(staminaSequence);

		moveSelector->AddChild(staminaFailer);
		moveSelector->AddChild(new MoveInDirectionOfVolume());

		Sequence* hookSequence = new Sequence();
		hookSequence->Initialize();
		hookSequence->AddChild(new UseHook());
		hookSequence->AddChild(new RaiseHook());
		hookSequence->AddChild(new MoveInDirectionOfVolume());

		moveSelector->AddChild(hookSequence);
		root->AddChild(moveSelector);

		bt.SetRoot(root);
	}

	void think(float dt)
	{
		bt.Tick(getParent(), dt, isHooked);
	}

	BehaviourTree bt;
	bool isHooked;
};

struct BoostPadResponseComponent : public ICollisionResponseComponent, public AutoLister<BoostPadResponseComponent>
{
	BoostPadResponseComponent(int id)
		: ICollisionResponseComponent(id)
	{

	}

	void endContact(IEntity * e)
	{

	};

	void beginContact(IEntity * e);
};


struct SlowShotResponseComponent : public ICollisionResponseComponent
{
	SlowShotResponseComponent(int id)
		: ICollisionResponseComponent(id)
	{
	}

	void endContact(IEntity * e)
	{
	
	};

	void beginContact(IEntity * e)
	{
		if (e)
		{
			if (e->getComponent<DirectionComponent>())
			{}
			else if (e->getComponent<BoostPadResponseComponent>())
			{}
			else
			{
				auto ai = getComponent<SeekAIComponent>();

				if (ai && ai->shooterID != e->ID)
				{
					getParent()->alive = false;
					e->getComponent<StateComponent>()->hit = true;
				}
			}
		}
	}
};

struct WebDropResponseComponent : public ICollisionResponseComponent
{
	WebDropResponseComponent(int id)
		: ICollisionResponseComponent(id)
	{}

	void endContact(IEntity * e)
	{

	};

	void beginContact(IEntity * e)
	{
		if (e)
		{
			auto staticBox = e->getComponent<StaticBodyComponent>();
			auto dynBox = e->getComponent<DynamicBodyComponent>();

			if (staticBox) {}
			else if (dynBox)
			{
				getParent()->alive = false;
				e->getComponent<Box2DComponent>()->body->SetLinearVelocity(b2Vec2(0, 0));
			}
		}
	};
};

struct DirectionVolumeCollisionResponseComponent : public ICollisionResponseComponent
{
	DirectionVolumeCollisionResponseComponent(int id)
		: ICollisionResponseComponent(id)
	{

	}

	void endContact(IEntity* e)
	{
		auto racePositionComponent = e->getComponent<RacePositionComponent>();

		if (racePositionComponent && racePositionComponent->volumeID == ID)
		{
			racePositionComponent->SetVolumeId(racePositionComponent->prevVolumeID);
		}
	};

	void beginContact(IEntity* e)
	{
		int volumeID = this->ID;
		auto racePositionComponent = e->getComponent<RacePositionComponent>();

		if (racePositionComponent && e->getComponent<ScoreComponent>()->alive)
		{
			racePositionComponent->SetVolumeId(volumeID);
		}
	};
};



struct PowerUpResponseComponent : public ICollisionResponseComponent, public AutoLister<PowerUpResponseComponent>
{
	PowerUpResponseComponent(int id)
		: ICollisionResponseComponent(id)
	{}

	void endContact(IEntity * e)
	{};

	void beginContact(IEntity * e)
	{}
};

struct PowerUpRespawnComponent: public IComponent, public AutoLister<PowerUpRespawnComponent>
{
	PowerUpRespawnComponent(int id, int x, int y)
		: IComponent(id)
		, tts(0)
		, isDead(false)
		, pos(b2Vec2(x, y))
	{}

	void ReSpawn() 
	{
		auto b = getComponent<Box2DComponent>();
		if (b)
		{
			auto body = b->body;
			body->SetTransform(pos, 0);
			isDead = false;
			tts = 0;
		}
	}

	void Die() 
	{
		auto b = getComponent<Box2DComponent>();
		if (b)
		{
			isDead = true;
		}
	}

	float tts;
	bool isDead;
	b2Vec2 pos;
};

