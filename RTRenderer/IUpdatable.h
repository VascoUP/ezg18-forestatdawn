#pragma once

class IUpdatable
{
public:
	virtual void SetUp() = 0;
	virtual void Update() = 0;
};
