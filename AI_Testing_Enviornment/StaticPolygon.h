#pragma once
#include "Components.h"

class StaticBox : public IEntity
{
public:
	StaticBox(int id, float x, float y, float w, float h) : IEntity(id,
	{
		new StaticBodyComponent(id, x, y, w, h, true)
	})
	{

	}

private:

};

class StaticPoly : public IEntity
{
public:
	StaticPoly(int id, std::vector<b2Vec2> points) : IEntity(id,
	{
		new StaticBodyComponent(id, points, true),
	})
	{
	};

private:

};

