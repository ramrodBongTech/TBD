#pragma once
#include "ECSInterfaces.h"
#include "Interactables.h"
#include "Dimensional.h"

class SwapSystem : public ISystem
{
public:
	void process(float dt) override
	{
		auto components = AutoList::get<SwapComponent>();
		for (auto& swap : components)
		{
			swap->ttl += dt;

			auto shooter = swap->getComponent<Box2DComponent>();
			auto target = swap->target;
			if (shooter && target)
			{
				//auto thisControllerComp = shooter->getComponent<IControllerComponent>();
				//auto targetControllerComp = target->getComponent<IControllerComponent>();
				auto thisAIComp = shooter->getComponent<PlayerAIComponent>();
				auto targetAIComp = target->getComponent<PlayerAIComponent>();

				auto obstacle = PhysicsSystem::RayCastToStaticObject(shooter->body->GetPosition(), target->getComponent<Box2DComponent>()->body->GetPosition(), 55);
				if (obstacle.first)
				{
					/*if (thisControllerComp)
					{
						thisControllerComp->isHooked = false;
						shooter->body->SetGravityScale(1);
					}
					else*/ if (thisAIComp)
					{
						thisAIComp->isHooked = false;
						shooter->body->SetGravityScale(1);
					}

					/*if (targetControllerComp)
					{
						targetControllerComp->isHooked = false;
						swap->target->getComponent<Box2DComponent>()->body->SetGravityScale(1);
					}
					else*/ if (targetAIComp)
					{
						targetAIComp->isHooked = false;
						swap->target->getComponent<Box2DComponent>()->body->SetGravityScale(1);
					}
					
					shooter->getParent()->deleteComponent<SwapComponent>();
				}
				else
				{
					swap->line->start = shooter->body->GetPosition();
					swap->line->end = swap->target->getComponent<Box2DComponent>()->body->GetPosition();

					float dis = Vector2D::Distance(Vector2D(shooter->body->GetPosition()), Vector2D(swap->target->getComponent<Box2DComponent>()->body->GetPosition()));
					if (swap->isShot)
					{
						swap->isShot = false;
					}
					else if (dis >= swap->tetherLength - 2)
					{
						/*if (thisControllerComp)
						{
							thisControllerComp->isHooked = false;
							shooter->body->SetGravityScale(1);
						}
						else*/ if(thisAIComp)
						{
							thisAIComp->isHooked = false;
							shooter->body->SetGravityScale(1);
						}

						/*if (targetControllerComp)
						{
							targetControllerComp->isHooked = false;
							swap->target->getComponent<Box2DComponent>()->body->SetGravityScale(1);
						}
						else*/ if (targetAIComp)
						{
							targetAIComp->isHooked = false;
							swap->target->getComponent<Box2DComponent>()->body->SetGravityScale(1);
						}

						shooter->getParent()->deleteComponent<SwapComponent>();
					}
				}
			}

			if (swap && swap->ttl >= 0.9f)
			{
				/*if (shooter->getComponent<IControllerComponent>())
				{
					shooter->getComponent<IControllerComponent>()->isHooked = false;
					shooter->body->SetGravityScale(1);
				}
				else*/ if (shooter->getComponent<PlayerAIComponent>())
				{
					shooter->getComponent<PlayerAIComponent>()->isHooked = false;
					shooter->body->SetGravityScale(1);
				}

				/*if (swap->target->getComponent<IControllerComponent>())
				{
					swap->target->getComponent<IControllerComponent>()->isHooked = false;
					swap->target->getComponent<Box2DComponent>()->body->SetGravityScale(1);
				}
				else*/ if (swap->target->getComponent<PlayerAIComponent>())
				{
					swap->target->getComponent<PlayerAIComponent>()->isHooked = false;
					swap->target->getComponent<Box2DComponent>()->body->SetGravityScale(1);
				}

				shooter->getParent()->deleteComponent<SwapComponent>();
			}
		}
	}
private:

};
