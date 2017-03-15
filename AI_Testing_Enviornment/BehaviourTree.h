#pragma once
#include "Composite.h"
#include "Leaf.h"
#include "Decorator.h"

class BehaviourTree : public Node
{
public:
	BehaviourTree()
		: root(nullptr)
	{}

	BehaviourTree(Node* rootNode)
		: root(rootNode)
	{}

	Status Update(IEntity* p, float dt, bool isHooked)
	{
		if (p != nullptr)
		{
			return root->Tick(p, dt, isHooked);
		}
		else
		{
			return Status::Failure;
		}
	}

	void SetRoot(Node* node) { root = node; }

private:
	Node* root;
};