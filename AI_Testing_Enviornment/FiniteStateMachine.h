#pragma once
#include <string>


class IState
{
public:
	virtual ~IState() {}
	virtual void update(float dt)
	{

	}
	virtual std::string getType()
	{
		return m_type;
	}
	virtual bool getDirection()
	{
		return m_left;
	}
protected:
	std::string m_type;
	bool m_left;
};

class Idle : public IState
{
public:
	Idle(bool);
private:
};

class Running : public IState
{
public:
	Running(bool);
private:
	
};

class Swinging : public IState
{
public:
	Swinging(bool);
private:
};

class Falling : public IState
{
public:
	Falling(bool);
private:
};

class Landing : public IState
{
public:
	Landing(bool);
private:
};

class Attacking : public IState
{
public:
	Attacking(bool);
private:
};

class Hitting : public IState
{
public:
	Hitting(bool);
private:
};

