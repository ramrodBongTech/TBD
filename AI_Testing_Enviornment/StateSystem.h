#pragma once
#include "ECSInterfaces.h"
#include "Components.h"

class StateSystem : public ISystem
{
public:
	StateSystem() {};

	void process(float dt) override
	{
		
		auto& components = AutoList::get<StateComponent>();
		for (auto& component : components)
		{
			using namespace ANIMATIONS::PLAYER;

			auto p = component->getParent();

			int id = p->getComponent<AnimationComponent>()->coulourID;

			if (m_lockedFrames == 0)
			{
				hitCheck(component, id);
				attackCheck(component, id);
			}

			

			//IDLING
			if (component->state->getType() == "idle"){
				if (runCheck(component, id)) {} //run
				else if (swingCheck(component, id)) {} //swing
				else if (fallCheck(component, id)) {} //fall
			}

			//RUNNING
			else if (component->state->getType() == "running"){
				if (idleCheck(component, id)) {}
				else if (swingCheck(component, id)) {} //swing
				else if (fallCheck(component, id)) {} //fall
				else if (runCheck(component, id)) {}
			}
			//SWINGING
			else if (component->state->getType() == "swinging"){
				if (!p->getComponent<HookComponent>()) //check if hooks exists, cannot leave swing untill not on a hook
				{
					if (landCheck(component, id)) {}
					else if (fallCheck(component, id)) {} //fall
					if (p->getComponent<DynamicBodyComponent>()->body->GetLinearVelocity().y == 0)
					{
						runCheck(component, id);
					}
					else
					{
						component->state = new Idle(component->state->getDirection());
					}
				}
			}
			//FALLING
			else if (component->state->getType() == "falling"){
				if (swingCheck(component, id)) {} //swing
				else if (landCheck(component, id)) {}
				else if (p->getComponent<DynamicBodyComponent>()->body->GetLinearVelocity().y == 0)
				{
					//runCheck(component, id);
				}
			}
			//LANDING
			else if (component->state->getType() == "landing")
			{
				if (landingToRunCheck(component, id)) {} //run
				else if (idleCheck(component, id)) {}
				else if (swingCheck(component, id)) {} //swing
			}
			//ATTACKING
			else if (component->state->getType() == "attacking")
			{
				if (m_lockedFrames == 0)
				{
					if (landingToRunCheck(component, id)) {} //run
					else if (swingCheck(component, id)) {} //swing
					else if (fallCheck(component, id)) {} //fall
				}

			}
			//HITTING
			else if (component->state->getType() == "hitting")
			{
				if (m_lockedFrames == 0)
				{
					if (landingToRunCheck(component, id)) {} //run
					else if (swingCheck(component, id)) {} //swing
					else if (fallCheck(component, id)) {} //fall
					else if (idleCheck(component, id)) {}
				}

			}

			if (m_lockedFrames > 0)
			{
				m_lockedFrames--;
			}
		}

	}
	//Colour 0 = Blue, 1 = Green, 2 = Red, 3 = Yellow
	bool idleCheck(StateComponent* component, int colour)
	{
	//	auto p = component->getParent();
	//	auto pc = p->getComponent<PlayerControllerComponent>();
	//	if (pc)
	//	{
	//		int contollerID = p->getComponent<PlayerControllerComponent>()->m_controllerId;
	//		if (InputManager::GetInstance()->GetLeftStickVectorNormal(contollerID).x == 0 && (p->getComponent<DynamicBodyComponent>()->body->GetLinearVelocity().x < 0.5 && p->getComponent<DynamicBodyComponent>()->body->GetLinearVelocity().x > -0.5))
	//		{
	//			if (!component->state->getDirection())
	//			{
	//				switch (colour)
	//				{
	//				case 0:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::IDLERIGHT);
	//					break;
	//				case 1:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::IDLERIGHT);
	//					break;
	//				case 2:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::IDLERIGHT);
	//					break;
	//				case 3:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::IDLERIGHT);
	//					break;
	//				}
	//			}
	//			else
	//			{
	//				switch (colour)
	//				{
	//				case 0:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::IDLELEFT);
	//					break;
	//				case 1:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::IDLELEFT);
	//					break;
	//				case 2:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::IDLELEFT);
	//					break;
	//				case 3:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::IDLELEFT);
	//					break;
	//				}
	//			}
	//			component->state = new Idle(component->state->getDirection());
	//			return true;
	//		}
	//	}
	//	else
	//	{
	//		if ((p->getComponent<DynamicBodyComponent>()->body->GetLinearVelocity().x < 0.5 && p->getComponent<DynamicBodyComponent>()->body->GetLinearVelocity().x > -0.5))
	//		{
	//			if (!component->state->getDirection())
	//			{
	//				switch (colour)
	//				{
	//				case 0:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::IDLERIGHT);
	//					break;
	//				case 1:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::IDLERIGHT);
	//					break;
	//				case 2:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::IDLERIGHT);
	//					break;
	//				case 3:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::IDLERIGHT);
	//					break;
	//				}
	//			}
	//			else
	//			{
	//				switch (colour)
	//				{
	//				case 0:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::IDLELEFT);
	//					break;
	//				case 1:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::IDLELEFT);
	//					break;
	//				case 2:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::IDLELEFT);
	//					break;
	//				case 3:
	//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::IDLELEFT);
	//					break;
	//				}
	//			}
	//			component->state = new Idle(component->state->getDirection());
	//			return true;
	//		}
	//	}
		return false;
	}
	bool runCheck(StateComponent* component, int colour)
	{
	//	auto p = component->getParent();

	//	auto pc = p->getComponent<PlayerControllerComponent>();
	//	if (pc)
	//	{
	//		int contollerID = pc->m_controllerId;

	//		if (InputManager::GetInstance()->GetLeftStickVectorNormal(contollerID).x > 0) //runright
	//		{
	//			//move right
	//			switch (colour)
	//			{
	//			case 0:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::RUNNINGRIGHT);
	//				break;
	//			case 1:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::RUNNINGRIGHT);
	//				break;
	//			case 2:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::RUNNINGRIGHT);
	//				break;
	//			case 3:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::RUNNINGRIGHT);
	//				break;
	//			}
	//			component->getComponent<AnimationComponent>()->animation.setFramesPerSecond(40);
	//			component->state = new Running(false);
	//			return true;
	//		}
	//		else if (InputManager::GetInstance()->GetLeftStickVectorNormal(contollerID).x < 0) //runleft
	//		{
	//			//run left
	//			//move right
	//			switch (colour)
	//			{
	//			case 0:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::RUNNINGLEFT);
	//				break;
	//			case 1:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::RUNNINGLEFT);
	//				break;
	//			case 2:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::RUNNINGLEFT);
	//				break;
	//			case 3:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::RUNNINGLEFT);
	//				break;
	//			}
	//			component->getComponent<AnimationComponent>()->animation.setFramesPerSecond(40);
	//			component->state = new Running(true);
	//			return true;
	//		}
	//	}
	//	else
	//	{
	//		if (p->getComponent<Box2DComponent>()->body->GetLinearVelocity().x > 0) //runright
	//		{
	//			//move right
	//			switch (colour)
	//			{
	//			case 0:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::RUNNINGRIGHT);
	//				break;
	//			case 1:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::RUNNINGRIGHT);
	//				break;
	//			case 2:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::RUNNINGRIGHT);
	//				break;
	//			case 3:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::RUNNINGRIGHT);
	//				break;
	//			}
	//			component->getComponent<AnimationComponent>()->animation.setFramesPerSecond(40);
	//			component->state = new Running(false);
	//			return true;
	//		}
	//		else if (p->getComponent<Box2DComponent>()->body->GetLinearVelocity().x < 0) //runleft
	//		{
	//			//run left
	//			//move right
	//			switch (colour)
	//			{
	//			case 0:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::RUNNINGLEFT);
	//				break;
	//			case 1:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::RUNNINGLEFT);
	//				break;
	//			case 2:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::RUNNINGLEFT);
	//				break;
	//			case 3:
	//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::RUNNINGLEFT);
	//				break;
	//			}
	//			component->getComponent<AnimationComponent>()->animation.setFramesPerSecond(40);
	//			component->state = new Running(true);
	//			return true;
	//		}
	//	}
		return false;
	}

	bool landingToRunCheck(StateComponent* component, int colour)
	{
		auto p = component->getParent();

		if (p->getComponent<DynamicBodyComponent>()->body->GetLinearVelocity().x > 0) //runright
		{
			//move right
			switch (colour)
			{
			case 0:
				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::RUNNINGRIGHT);
				break;
			case 1:
				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::RUNNINGRIGHT);
				break;
			case 2:
				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::RUNNINGRIGHT);
				break;
			case 3:
				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::RUNNINGRIGHT);
				break;
			}
			component->getComponent<AnimationComponent>()->animation.setFramesPerSecond(40);
			component->state = new Running(false);
			return true;
		}
		else if (p->getComponent<DynamicBodyComponent>()->body->GetLinearVelocity().x < 0) //runleft
		{
			//run left
			//move right
			switch (colour)
			{
			case 0:
				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::RUNNINGLEFT);
				break;
			case 1:
				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::RUNNINGLEFT);
				break;
			case 2:
				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::RUNNINGLEFT);
				break;
			case 3:
				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::RUNNINGLEFT);
				break;
			}
			component->getComponent<AnimationComponent>()->animation.setFramesPerSecond(40);
			component->state = new Running(true);
			return true;
		}

		return false;
	}
	bool swingCheck(StateComponent* component, int colour)
	{
		//auto p = component->getParent();

		//auto pc = p->getComponent<PlayerControllerComponent>();
		//if (pc)
		//{
		//	int contollerID = p->getComponent<PlayerControllerComponent>()->m_controllerId;
		//	if (p->getComponent<HookComponent>())  //check if hooks exists, cannot enter swing until on a hook
		//	{
		//		if (InputManager::GetInstance()->GetLeftStickVectorNormal(contollerID).x >= 0)
		//		{
		//			//move right
		//			switch (colour)
		//			{
		//			case 0:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::FALLRIGHT);
		//				break;
		//			case 1:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::FALLRIGHT);
		//				break;
		//			case 2:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::FALLRIGHT);
		//				break;
		//			case 3:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::FALLRIGHT);
		//				break;
		//			}

		//			component->state = new Swinging(false);
		//		}
		//		else
		//		{
		//			//move right
		//			switch (colour)
		//			{
		//			case 0:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::FALLLEFT);
		//				break;
		//			case 1:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::FALLLEFT);
		//				break;
		//			case 2:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::FALLLEFT);
		//				break;
		//			case 3:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::FALLLEFT);
		//				break;
		//			}
		//			component->state = new Swinging(true);
		//		}
		//		component->getComponent<AnimationComponent>()->animation.setFramesPerSecond(20);
		//		return true;
		//	}
		//	else
		//	{
		//		if (p->getComponent<HookComponent>())  //check if hooks exists, cannot enter swing until on a hook
		//		{
		//			if (p->getComponent<Box2DComponent>()->body->GetLinearVelocity().x >= 0)
		//			{
		//				//move right
		//				switch (colour)
		//				{
		//				case 0:
		//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::FALLRIGHT);
		//					break;
		//				case 1:
		//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::FALLRIGHT);
		//					break;
		//				case 2:
		//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::FALLRIGHT);
		//					break;
		//				case 3:
		//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::FALLRIGHT);
		//					break;
		//				}

		//				component->state = new Swinging(false);
		//			}
		//			else
		//			{
		//				//move right
		//				switch (colour)
		//				{
		//				case 0:
		//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::FALLLEFT);
		//					break;
		//				case 1:
		//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::FALLLEFT);
		//					break;
		//				case 2:
		//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::FALLLEFT);
		//					break;
		//				case 3:
		//					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::FALLLEFT);
		//					break;
		//				}
		//				component->state = new Swinging(true);
		//			}
		//			component->getComponent<AnimationComponent>()->animation.setFramesPerSecond(20);
		//			return true;
		//		}
		//	}
		//}
		return false;
	}
	bool fallCheck(StateComponent* component, int colour)
	{
		auto p = component->getParent();

		if (p->getComponent<DynamicBodyComponent>()->body->GetLinearVelocity().y > 0)
		{
			if (p->getComponent<DynamicBodyComponent>()->body->GetLinearVelocity().x >= 0)
			{
				switch (colour)
				{
				case 0:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::FALLRIGHT);
					break;
				case 1:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::FALLRIGHT);
					break;
				case 2:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::FALLRIGHT);
					break;
				case 3:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::FALLRIGHT);
					break;
				}
				component->state = new Falling(false);
				return true;
			}
			else
			{
				switch (colour)
				{
				case 0:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::FALLLEFT);
					break;
				case 1:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::FALLLEFT);
					break;
				case 2:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::FALLLEFT);
					break;
				case 3:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::FALLLEFT);
					break;
				}
				component->state = new Falling(true);
				return true;
			}
		}	
		return false;
	}
	bool landCheck(StateComponent* component, int colour)
	{
		auto p = component->getParent();
		if (p->getComponent<DynamicBodyComponent>()->body->GetLinearVelocity().y == 0)
		{
			if (p->getComponent<DynamicBodyComponent>()->body->GetLinearVelocity().x >= 0)
			{
				switch (colour)
				{
				case 0:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::LANDRIGHT);
					break;
				case 1:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::LANDRIGHT);
					break;
				case 2:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::LANDRIGHT);
					break;
				case 3:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::LANDRIGHT);
					break;
				}
				component->state = new Landing(false);
				return true;
			}
			else
			{
				switch (colour)
				{
				case 0:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::LANDLEFT);
					break;
				case 1:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::LANDLEFT);
					break;
				case 2:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::LANDLEFT);
					break;
				case 3:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::LANDLEFT);
					break;
				}
				component->state = new Landing(true);
				return true;
			}
		}
		return false;
	}
	bool attackCheck(StateComponent* component, int colour)
	{
		auto p = component->getParent();
		
			if (p->getComponent<AbilityComponent>()->ability == p->getComponent<AbilityComponent>()->NONE && p->getComponent<AbilityComponent>()->canAnimate) //this for abilities check ifthe player had one, if the still had one then animate is not
			{
				p->getComponent<AbilityComponent>()->canAnimate = false;
			if (!component->state->getDirection())
			{
				switch (colour)
				{
				case 0:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::ATTACKRIGHT);
					break;
				case 1:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::ATTACKRIGHT);
					break;
				case 2:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::ATTACKRIGHT);
					break;
				case 3:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::ATTACKRIGHT);
					break;
				}
				m_lockedFrames = 20;
				component->state = new Attacking(false);
				return true;
			}		
			else
			{
				switch (colour)
				{
				case 0:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::ATTACKLEFT);
					break;
				case 1:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::ATTACKLEFT);
					break;
				case 2:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::ATTACKLEFT);
					break;
				case 3:
					component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::ATTACKLEFT);
					break;
				}
				m_lockedFrames = 20;
				component->state = new Attacking(true);
				return true;
			}
		}

		return false;
	} 
	bool hitCheck(StateComponent* component, int colour)
	{
		//auto p = component->getParent();

		//auto pc = p->getComponent<PlayerControllerComponent>();
		//auto pa = p->getComponent<PlayerAIComponent>();
		//if (pc || pa)
		//{
		//	auto objects = AutoList::get<DynamicBodyComponent>();
		//	if (p->getComponent<StateComponent>()->hit == true) //this for abilities check ifthe player had one, if the still had one then animate is not
		//	{
		//		p->getComponent<StateComponent>()->hit = false;
		//		if (!component->state->getDirection())
		//		{
		//			switch (colour)
		//			{
		//			case 0:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::HITRIGHT);
		//				break;
		//			case 1:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::HITRIGHT);
		//				break;
		//			case 2:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::HITRIGHT);
		//				break;
		//			case 3:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::HITRIGHT);
		//				break;
		//			}
		//			m_lockedFrames = 100;
		//			component->state = new Hitting(false);
		//			return true;
		//		}
		//		else
		//		{
		//			switch (colour)
		//			{
		//			case 0:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::BLUE::HITLEFT);
		//				break;
		//			case 1:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::GREEN::HITLEFT);
		//				break;
		//			case 2:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::RED::HITLEFT);
		//				break;
		//			case 3:
		//				component->getComponent<AnimationComponent>()->animation.changeAnimation(ANIMATIONS::PLAYER::YELLOW::HITLEFT);
		//				break;
		//			}
		//			m_lockedFrames = 100;
		//			component->state = new Hitting(true);
		//			return true;
		//		}
		//	}
			return false;
		//}
	}
private:
	int m_lockedFrames = 0; //Amount of frames any animation is locked in for. If > 0 decrements every cycle


};
