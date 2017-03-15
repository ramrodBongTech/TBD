#pragma once
#include "ECSInterfaces.h"
#include "box2d\Box2D.h"
#include "Vector2D.h"
#include "Animation.h"
#include "FiniteStateMachine.h"


struct AnimationComponent : public IComponent, public AutoLister<AnimationComponent>
{
	AnimationComponent(int objectId, string animationName, int colID)
		: IComponent(objectId)
		, animation(Animation(animationName))
		, coulourID(colID){};
	Animation animation;
	int coulourID;
};

struct CountdownComponent : public IComponent, public AutoLister<CountdownComponent>
{
	CountdownComponent(int objectId, string animationName)
		: IComponent(objectId)
		, animation(Animation(animationName))
		, timeToDisplay(0)
	{
		animation.setFramesPerSecond(2.0f);
	};
	Animation animation;
	float timeToDisplay;
};

struct HudComponent : public IComponent, public AutoLister<HudComponent>
{
	HudComponent(int objectId, string animationName)
		: IComponent(objectId)
		, animation(Animation(animationName))
		, spinTime(0) {};
	Animation animation;
	float spinTime;
};

struct DirectionArrowComponent : public IComponent, public AutoLister<DirectionArrowComponent>
{
	DirectionArrowComponent(int objectId, std::string key = "placeholder")
		: IComponent(objectId)
		, textureKey(key)
		, angle(0) {};

	std::string textureKey;
	float angle;
};

struct StateComponent : public IComponent, public AutoLister<StateComponent>
{
	StateComponent(int id)
		: IComponent(id)
		, state(new Idle(false))
		, colour("blue")
		, hit(false) {};

	IState *state;
	std::string colour;
	bool hit;
};



struct LineComponent : public IComponent, AutoLister<LineComponent>
{
	LineComponent(int id, Vector2D _start, Vector2D _end) 
		:	IComponent(id)
		,	start(_start)
		,	end(_end) 
	{};
	Vector2D start, end;

};