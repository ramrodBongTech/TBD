#pragma once
#include "ECSInterfaces.h"
#include "Vector2D.h"
#include "box2d\Box2D.h"
#include "PhysicsSystem.h"

struct Box2DComponent : public AutoLister<Box2DComponent>, public IComponent
{
	Box2DComponent(int id, float x, float y, float width, float height, b2BodyType type=b2_staticBody, bool fixedRotation = true) 
		:	IComponent(id) 
		,	size(width, height)
	{
		b2BodyDef bodyDef;

		bodyDef.position.Set(x + width / 2.f, y + height / 2.f);

		b2PolygonShape shape;
		shape.SetAsBox(width / 2.f, height / 2.f);
		bodyDef.type = type;

		if (type == b2_staticBody)
		{
			body = PhysicsSystem::World().CreateBody(&bodyDef);
			fixture = body->CreateFixture(&shape, 0.5f);
		}
		else if (type == b2_dynamicBody)
		{
			bodyDef.fixedRotation = true;
			body = PhysicsSystem::World().CreateBody(&bodyDef);
			b2FixtureDef afixture;
			afixture.shape = &shape;
			afixture.density = 1.0f;
			afixture.friction = 0.0f;
			fixture = body->CreateFixture(&afixture);
		}
		else if (type == b2_kinematicBody)
		{
			bodyDef.fixedRotation = true;
			body = PhysicsSystem::World().CreateBody(&bodyDef);
			b2FixtureDef afixture;
			afixture.shape = &shape;
			afixture.density = 1.0f;
			afixture.friction = 0.1f;
			afixture.isSensor = true;
			fixture = body->CreateFixture(&afixture);
		}
		body->SetUserData(this);
	}

	Box2DComponent(int id, std::vector<b2Vec2> points, b2BodyType type = b2_staticBody, bool fixedRotation = true) : IComponent(id)
	{
		b2BodyDef bodyDef;

		b2Vec2 vec[16];

		for (int i = 0; i < points.size(); i++)
		{
			vec[i].Set(points.at(i).x, points.at(i).y);
		}

		b2PolygonShape shape;
		shape.Set(&vec[0], points.size());

		bodyDef.type = type;

		if (type == b2_staticBody)
		{	
			body = PhysicsSystem::World().CreateBody(&bodyDef);
			body->CreateFixture(&shape, 0.5f);
		}
		else if (type == b2_kinematicBody)
		{
			bodyDef.fixedRotation = fixedRotation;
			body = PhysicsSystem::World().CreateBody(&bodyDef);
			b2FixtureDef afixture;
			afixture.shape = &shape;
			afixture.density = 0.5;
			afixture.friction = 0.5;
			afixture.restitution = 0.5;
			fixture = body->CreateFixture(&afixture);
		}
		else if (type == b2_dynamicBody)
		{
			bodyDef.fixedRotation = fixedRotation;
			body = PhysicsSystem::World().CreateBody(&bodyDef);
			b2FixtureDef afixture;
			afixture.shape = &shape;
			afixture.density = 1.0;
			afixture.friction = 0.1;
			afixture.isSensor = true;
			fixture = body->CreateFixture(&afixture);
		}
		body->SetUserData(this);
	}

	virtual ~Box2DComponent()
	{
		PhysicsSystem::World().DestroyBody(body);
	};

	b2Body* body;
	b2Fixture* fixture;
	Vector2D size;
};



struct StaticBodyComponent : public Box2DComponent, public AutoLister<StaticBodyComponent>
{
	StaticBodyComponent(int id, float x, float y, float width, float height, bool fixedRotation=true)
		:	Box2DComponent(id, x, y, width, height, b2_staticBody, fixedRotation) 
	{
	}
	StaticBodyComponent(int id, std::vector<b2Vec2> points, bool fixedRotation = true)
		:	Box2DComponent(id, points, b2_staticBody, fixedRotation)
	{
	}
};

struct DynamicBodyComponent : public Box2DComponent, public AutoLister<DynamicBodyComponent>
{
	DynamicBodyComponent(int id, float x, float y, float width, float height, bool fixedRotation = true)
		: Box2DComponent(id, x, y, width, height, b2_dynamicBody, fixedRotation)
	{
	}
};

struct KinematicBodyComponent : public Box2DComponent, public AutoLister<KinematicBodyComponent>
{
	KinematicBodyComponent(int id, float x, float y, float width, float height, bool fixedRotation = true)
		: Box2DComponent(id, x, y, width, height, b2_kinematicBody, fixedRotation)
	{
	}
};

struct SensorComponent : public KinematicBodyComponent, public AutoLister<SensorComponent>
{
	SensorComponent(int id, float x, float y, float width, float height)
		: KinematicBodyComponent(id, x, y, width, height)
	{
		fixture->SetSensor(true);
		fixture->SetFriction(0);
		fixture->SetDensity(0);
	}
};

struct SlowShotComponent : public DynamicBodyComponent, public AutoLister<SlowShotComponent>
{
	SlowShotComponent(int id, float x, float y, float width, float height, bool fixedRotation = true) : DynamicBodyComponent(id, x, y, width, height)
	{
		body->SetGravityScale(0);
		fixture->SetSensor(true);
		fixture->SetFriction(0);
		fixture->SetDensity(0);
	}
};

struct WebDropComponent : public StaticBodyComponent, public AutoLister<WebDropComponent>
{
	WebDropComponent(int id, float x, float y, float width, float height, bool fixedRotation = true)
		: StaticBodyComponent(id, x, y, width, height)
	{
		fixture->SetFriction(10);
		fixture->SetDensity(10);
	}
};

struct DirectionComponent : public IComponent, public AutoLister<DirectionComponent>
{
	DirectionComponent(int id, Vector2D direction)
		: IComponent(id)
		, m_direction(direction)
	{
	}

	Vector2D m_direction;
};
