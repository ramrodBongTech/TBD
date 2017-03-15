#pragma once
#include <vector>
#include <memory>
#include "ECSInterfaces.h"

class Node
{
public:
	enum class Status
	{
		Invalid,
		Success,
		Failure,
		Running,
	};

	virtual ~Node() {}

	virtual Status Update(IEntity* p, float dt, bool isHooked) = 0;
	virtual void Initialize() {}
	virtual void Terminate(Status status) {}

	Status Tick(IEntity* p, float dt, bool isHooked)
	{
		if (status != Status::Running) {
			Initialize();
		}

		status = Update(p, dt, isHooked);

		if (status != Status::Running) {
			Terminate(status);
		}

		return status;
	}

	virtual void Reset() { status = Status::Invalid; }

protected:
	Status status = Status::Invalid;
};