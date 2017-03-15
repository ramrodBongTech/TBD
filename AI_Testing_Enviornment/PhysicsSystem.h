#pragma once
#include "ECSInterfaces.h"
#include "box2d\Box2D.h"
#include "Vector2D.h"

class Box2DComponent;
class PhysicsSystem : public ISystem
{
public:
	void process(float dt) override;

	static std::pair<IEntity*, Vector2D> RayCastToStaticObject(Vector2D start, Vector2D end, float maxLength=100);

	static Vector2D RayCast(Vector2D start, Vector2D end, float maxLength = 100);

	static b2Vec2& Gravity();

	static b2World& World();

	void DecelerateBoost(float dt, Box2DComponent *b);

	void setPausePhysics(bool b);

private:

	bool pausePhysics;
};
