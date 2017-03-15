#pragma once
#include "../dependancies/MetaSystem/MetaSystem.h"
#include <algorithm>

template<typename T>
T* getComponentById(int id)
{
	auto& list = AutoList::get<T>();
	for (auto& el : list)
	{
		if (el->ID == id)
		{
			return el;
		}
	}

	return nullptr;
}

class ISystem
{
private:
	float m_timeSinceLastUpdate;
	const float maxTimeTillUpdate;
public:
	ISystem(float ticksPerSecond = std::numeric_limits<float>::max()) : maxTimeTillUpdate(1.f / ticksPerSecond), m_timeSinceLastUpdate(0) {}
	virtual void update(float dt)
	{
		m_timeSinceLastUpdate += dt;
		if (m_timeSinceLastUpdate >= maxTimeTillUpdate)
		{
			m_timeSinceLastUpdate = 0;
			this->process(dt);
		}
	}

	virtual void process(float dt) = 0;
};

class IEntity;

struct IComponent : public AutoLister<IComponent>
{
	IComponent(int id) : ID(id) {};

	template<typename T>
	T* getComponent()
	{
		return getComponentById<T>(ID);
	}

	IEntity* getParent()
	{
		return getComponentById<IEntity>(ID);
	}

	virtual ~IComponent() {};
	const int ID;
};

class IEntity : public AutoLister<IEntity>
{
public:
	IEntity(int id, std::vector<IComponent*> list) : ID(id), m_components(list)
	{
		alive = true;
	}

	template<typename T>
	T* getComponent()
	{
		return getComponentById<T>(ID);
	}

	virtual ~IEntity()
	{
		for (auto i : m_components)
		{
			delete i;
		}
		m_components.clear();
	}


	void AddComponent(IComponent* component)
	{
		m_components.push_back(component);
	}

	template<typename T>
	bool deleteComponent()
	{
		auto component = getComponentById<T>(ID);

		if (component)
		{
			delete component;
			m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
			return true;
		}
		return false;
	}

	const int ID;
	bool alive;
protected:
	std::vector<IComponent*> m_components;

};