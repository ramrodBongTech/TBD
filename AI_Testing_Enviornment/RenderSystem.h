#pragma once

#include "sdl\SDL.h"
#include "ECSInterfaces.h"
#include "Renderer.h"
#include "LevelLoader.h"

class RenderSystem : public ISystem
{
public:
	void init(Renderer * r);
	void process(float dt) override;
	void setLevel(LEVELS levelKey);

private:
	Renderer* m_renderer;
	std::string m_levelKey;
	Rect m_levelRect;
};
