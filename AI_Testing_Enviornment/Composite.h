#pragma once
#include "Node.h"

class Composite : public Node
{
public:
	virtual ~Composite() { children.clear(); }

	void AddChild(Node* child) { children.push_back(child); }
	bool HasNoChildren() const { return children.empty(); }
	int GetIndex() const { return index; }

protected:
	std::vector<Node*> children;
	int index = 0;
};

class Selector : public Composite
{
public:
	void Initialize() override
	{
		index = 0;
	}

	Status Update(IEntity* p, float dt, bool isHooked)
	{
		if (HasNoChildren()) 
		{
			return Status::Success;
		}

		while (1) 
		{
			auto& child = children.at(index);
			auto status = child->Tick(p, dt, isHooked);

			if (status != Status::Failure) 
			{
				return status;
			}

			if (++index == children.size()) 
			{
				return Status::Failure;
			}
		}
	}
};

class Sequence : public Composite
{
public:
	void Initialize() override
	{
		index = 0;
	}

	Status Update(IEntity* p, float dt, bool isHooked)
	{
		if (HasNoChildren()) 
		{
			return Status::Failure;
		}

		while (1) 
		{
			auto& child = children.at(index);
			auto status = child->Tick(p, dt, isHooked);

			if (status != Status::Success) 
			{
				return status;
			}

			if (++index == children.size()) 
			{
				return Status::Success;
			}
		}
	}
};