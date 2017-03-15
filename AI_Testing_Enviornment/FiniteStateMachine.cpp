#include "stdafx.h"
#include "FiniteStateMachine.h"

Idle::Idle(bool left)
{
	m_left = left;
	m_type = "idle";
}

Running::Running(bool left)
{
	m_left = left;
	m_type = "running";
}

Swinging::Swinging(bool b)
{
	m_left = b;
	m_type = "swinging";
}

Falling::Falling(bool b)
{
	m_left = b;
	m_type = "falling";
}

Landing::Landing(bool b)
{
	m_left = b;
	m_type = "landing";
}

Attacking::Attacking(bool b)
{
	m_left = b;
	m_type = "attacking";
}

Hitting::Hitting(bool b)
{
	m_left = b;
	m_type = "hitting";
}
