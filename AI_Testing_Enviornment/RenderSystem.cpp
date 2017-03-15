#include "stdafx.h"
#include "RenderSystem.h"
#include "Drawables.h"
#include "Dimensional.h"
#include "ResourceManager.h"
#include "Entities.h"

void RenderSystem::init(Renderer * r)
{
	m_renderer = r;
	m_levelKey = "mapone";
	m_levelRect = Rect(0, 0, 400, 395);

}

void RenderSystem::process(float dt)
{
	m_renderer->clear(Colour(128, 128, 64));
	m_renderer->drawTexture(ResourceManager::getInstance()->getTextureByKey(m_levelKey), m_levelRect);

	auto& softBoxes = AutoList::get<SoftBox>();
	for (auto& component : softBoxes)
	{
		m_renderer->drawTexture(ResourceManager::getInstance()->getTextureByKey("web"), Rect(-(component->getComponent<SensorComponent>()->size / 2)+component->getComponent<SensorComponent>()->body->GetPosition(), component->getComponent<SensorComponent>()->size));
	}

	auto& webDrops = AutoList::get<WebDrop>();
	for (auto& component : webDrops)
	{
		m_renderer->drawTexture(ResourceManager::getInstance()->getTextureByKey("web"), Rect(-(component->getComponent<StaticBodyComponent>()->size / 2) + component->getComponent<StaticBodyComponent>()->body->GetPosition(), component->getComponent<StaticBodyComponent>()->size));
	}

	auto& boostPads = AutoList::get<BoostPad>();
	for (auto& component : boostPads)
	{
		m_renderer->drawTexture(ResourceManager::getInstance()->getTextureByKey("boost"), Rect(-(component->getComponent<SensorComponent>()->size / 2) + component->getComponent<SensorComponent>()->body->GetPosition(), component->getComponent<SensorComponent>()->size));
	}

	auto& powerups = AutoList::get<PowerUp>();
	for (auto& component : powerups)
	{
		m_renderer->drawTexture(ResourceManager::getInstance()->getTextureByKey("boxrandom"), Rect(-(component->getComponent<SensorComponent>()->size / 2) + component->getComponent<SensorComponent>()->body->GetPosition(), component->getComponent<SensorComponent>()->size));
	}

	auto& slowShots = AutoList::get<SlowShot>();
	for (auto& component : slowShots)
	{
		Vector2D angle = component->getComponent<SlowShotComponent>()->body->GetLinearVelocity();
		m_renderer->drawTextureWithAngle(ResourceManager::getInstance()->getTextureByKey("webattack"), Rect(-(component->getComponent<SlowShotComponent>()->size / 2) + component->getComponent<SlowShotComponent>()->body->GetPosition(), component->getComponent<SlowShotComponent>()->size), Vector2D::AngleDeg(angle, Vector2D(0,0)));
	}

	{
		auto& components = AutoList::get<LineComponent>();
		for (auto& component : components)
		{
			m_renderer->drawLine(component->start, component->end);
		}
	}
	{
		auto& components = AutoList::get<AnimationComponent>();
		for (auto& component : components)
		{
			if (component->getParent()->getComponent<ScoreComponent>())
			{
				auto p = component->getParent();
				auto b = p->getComponent<Box2DComponent>();
				if (p->getComponent<ScoreComponent>()->alive)
				{
					component->animation.drawAtPosition(m_renderer, Vector2D(b->body->GetPosition().x - b->size.width * 2, b->body->GetPosition().y - b->size.height * 2), Vector2D(b->size * 4), 0);
				}

			}
			else
			{
				auto p = component->getParent();
				auto b = p->getComponent<Box2DComponent>();
				component->animation.drawAtPosition(m_renderer, Vector2D(b->body->GetPosition().x - b->size.width * 2, b->body->GetPosition().y - b->size.height * 2), Vector2D(b->size * 4), 0);
			}

		}
	}
	Rect hudBar = Rect(0, 0, 1280, 110);

	m_renderer->drawRectHud(hudBar, Colour(32, 32, 32, 255));

	auto& scores = AutoList::get<AnimationComponent>();
	for (auto& component : scores)
	{
		SDL_Texture* tex = ResourceManager::getInstance()->getTextureByKey("placeholder");
		if (component->coulourID == 0)
		{
			tex = ResourceManager::getInstance()->getTextureByKey("bluehud");
		}
		else if (component->coulourID == 1)
		{
			tex = ResourceManager::getInstance()->getTextureByKey("greenhud");
		}
		else if (component->coulourID == 2)
		{
			tex = ResourceManager::getInstance()->getTextureByKey("redhud");
		}
		else if (component->coulourID == 3)
		{
			tex = ResourceManager::getInstance()->getTextureByKey("yellowhud");
		}

		Rect drawPos;

		if (component->coulourID < 2)
		{
			drawPos = Rect(160 * component->coulourID, 3, 150, 100);
		}
		else
		{
			drawPos = Rect( 1280 - (160 * (component->coulourID - 1)), 3, 150, 100);
		}
		

		int rounds = component->getParent()->getComponent<ScoreComponent>()->rounds;

		for (int i = 0; i < 3; i++)
		{
			if (rounds > i)
			{
				Rect counterPos = Rect((drawPos.pos.x + 4) + 50 * i, 16, 42, 42);
				m_renderer->drawHud(ResourceManager::getInstance()->getTextureByKey("greencounter"), counterPos);
			}
			else
			{
				Rect counterPos = Rect((drawPos.pos.x + 4) + 50 * i, 16, 42, 42);
				m_renderer->drawHud(ResourceManager::getInstance()->getTextureByKey("redcounter"), counterPos);
			}

		}

		Rect abilityPos = Rect((drawPos.pos.x) + 51 * 2, 16, 42, 42);

		auto ability = component->getParent()->getComponent<AbilityComponent>();
		if (ability->ability == ability->WEB_DROP)
		{
			m_renderer->drawHud(ResourceManager::getInstance()->getTextureByKey("webIcon"), abilityPos);
		}
		else if (ability->ability == ability->SWAP_SHOT)
		{
			m_renderer->drawHud(ResourceManager::getInstance()->getTextureByKey("hookIcon"), abilityPos);
		}
		else if (ability->ability == ability->SLOW_SHOT)
		{
			m_renderer->drawHud(ResourceManager::getInstance()->getTextureByKey("shotIcon"), abilityPos);
		}
		else
		{
			m_renderer->drawHud(ResourceManager::getInstance()->getTextureByKey("boxrandom"), abilityPos);
		}

		if (component->getParent()->getComponent<HudComponent>()->spinTime > 0)
		{
			component->getParent()->getComponent<HudComponent>()->animation.drawAtHudPosition(m_renderer, abilityPos.pos, abilityPos.size, 0);
			component->getParent()->getComponent<HudComponent>()->spinTime -= dt;
		}

		auto& Arrows = AutoList::get<DirectionArrowComponent>();
		for (auto& Arrow : Arrows)
		{
			m_renderer->drawTextureWithAngleHud(ResourceManager::getInstance()->getTextureByKey(Arrow->textureKey), Rect(600, 20, 80, 80), Arrow->angle);
		}
		
		auto& Countdowns = AutoList::get<CountdownComponent>();
		for (auto& Countdown : Countdowns)
		{
			if (Countdown->timeToDisplay > 0)
			{
				Countdown->animation.drawAtHudPosition(m_renderer, Vector2D(590, 310), Vector2D(100, 100), 0);
				Countdown->timeToDisplay -= dt;
			}	
		}

		Rect staminaRect = Rect(drawPos.pos.x, 70, 1 * component->getParent()->getComponent<StaminaComponent>()->stamina * 1.5f, 20);

		m_renderer->drawHud(ResourceManager::getInstance()->getTextureByKey("stamina"), staminaRect);

		m_renderer->drawHud(tex, drawPos);
	}
	
	m_renderer->present();
}

void RenderSystem::setLevel(LEVELS levelKey)
{
	if (levelKey == LEVELS::LEVEL1) 
	{
		m_levelKey = "mapone";
		m_levelRect = Rect(0, 0, 400, 395);
	}
	else if (levelKey == LEVELS::LEVEL2) {
		m_levelKey = "maptwo";
		m_levelRect = Rect(0, 0, 328, 164);
	}
	else if (levelKey == LEVELS::LEVEL3) {
		m_levelKey = "mapthree";
		m_levelRect = Rect(0, 0, 338, 146);
	}
	else if (levelKey == LEVELS::LEVEL4) {
		m_levelKey = "mapfour";
		m_levelRect = Rect(0, 0, 295, 152);
	}
}
