#include "stdafx.h"
#include "Leaf.h"
#include "EntityFactory.h"
#include "RaceManager.h"

UseAbility::UseAbility() {}

UseAbility::~UseAbility() {}

Node::Status UseAbility::Update(IEntity* p, float dt, bool isHooked)
{
	if (!isHooked)
	{
		auto a = p->getComponent<AbilityComponent>();
		auto c = p->getComponent<Box2DComponent>();
		if (a && c)
		{
			const auto none = a->NONE;
			const auto webDrop = a->WEB_DROP;
			const auto slowShot = a->SLOW_SHOT;
			const auto swapShot = a->SWAP_SHOT;

			switch(a->ability)
			{
			case none:
				break;
			case webDrop:
				//EntityFactory::SpawnWebDrop(c->body->GetPosition().x, c->body->GetPosition().y, 1, 1);
				a->ability = a->NONE;
				//notify(Observer::DROP);
				return Status::Success;
			case slowShot:
				EntityFactory::SpawnSlowShot(c->body->GetPosition().x, c->body->GetPosition().y - 1, 1, 1, p->ID);
				a->ability = a->NONE;
				//notify(Observer::SHOOT);
				return Status::Success;
			case swapShot:
				auto h = p->getComponent<HookComponent>();
				if (h)
				{
					p->deleteComponent<HookComponent>();
				}
				vector<Player*> players = RaceManager::getInstance()->getPlayers();
				int targetID = 1;
				for (int i = 0; i < players.size(); i++)
				{
					if (i + 1 == players.size())
					{
					}
					else if (players[i]->ID == p->ID)
					{
						auto targetBody = players[i + 1]->getComponent<Box2DComponent>()->body;
						auto obstacle = PhysicsSystem::RayCastToStaticObject(c->body->GetPosition(), targetBody->GetPosition(), 50);
					if (!obstacle.first)
					{
						b2Vec2 dis = (c->body->GetPosition() - targetBody->GetPosition());
						p->AddComponent(new SwapComponent(p->ID, c->body->GetPosition(), targetBody->GetPosition(), c->body, players[i + 1]));
						auto hook = players[i + 1]->getComponent<HookComponent>();
						if (hook)
						{
							players[i + 1]->deleteComponent<HookComponent>();
						}
						targetBody->SetLinearVelocity(b2Vec2(0, 0));
						targetBody->ApplyForceToCenter(b2Vec2(dis.x * 100000, dis.y * 100000), true);
						c->body->ApplyForceToCenter(b2Vec2(-dis.x * 100000, -dis.y * 100000), true);
						isHooked = true;
						players[i + 1]->getComponent<PlayerAIComponent>()->isHooked = true;
						
						targetBody->SetGravityScale(0);
						c->body->SetGravityScale(0);
					}
					i = players.size();
				}
				}
				a->ability = a->NONE;
				return Status::Success;
			}
		}
	}
	return Status::Failure;
}

MoveInDirectionOfVolume::MoveInDirectionOfVolume() {}

MoveInDirectionOfVolume::~MoveInDirectionOfVolume() {}

Node::Status MoveInDirectionOfVolume::Update(IEntity* p, float dt, bool isHooked)
{
	if (!isHooked)
	{
		auto b = p->getComponent<Box2DComponent>();
		auto a = p->getComponent<AccelerationComponent>();
		auto rp = p->getComponent<RacePositionComponent>();

		if (a && b && rp)
		{
			auto dirVol = getComponentById<DirectionVolume>(rp->volumeID);

			if (dirVol)
			{
				auto dirComp = dirVol->getComponent<DirectionComponent>();
				if (dirComp)
				{
					auto direction = dirComp->m_direction;
					if (direction.y == 0)
					{
						auto obstacle = PhysicsSystem::RayCastToStaticObject(b->body->GetPosition(), Vector2D(b->body->GetPosition()) + Vector2D(direction.x * 1000, 0));
						if (obstacle.first)
						{
							float distance = Vector2D::Distance(Vector2D(b->body->GetPosition()), obstacle.second);
							if (distance < 8)
							{
								float xVelocity = b->body->GetLinearVelocity().x;
								float xRay = 0;
								if (xVelocity > 0) { xRay = 1000; }
								else if (xVelocity < 0) { xRay = -1000; }
								auto intersection = PhysicsSystem::RayCastToStaticObject(b->body->GetPosition(), Vector2D(b->body->GetPosition()) + Vector2D(xRay, -1000));
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
								return Status::Running;
							}
							b->body->ApplyForceToCenter(b2Vec2(direction.x * a->acceleration, 0), true);
							return Status::Running;
						}
					}
					else
					{
						if (direction.x == 0)
						{
							if (direction.y > 0)
							{
								return Status::Success;
							}
							else if (direction.y < 0)
							{
								return Status::Failure;
							}
						}
					}
				}
			}
		}
	}
	return Status::Failure;
}