#pragma once

#include <stdio.h>

class Renderable
{
public:
	Renderable();
	void Register(bool toRegister);
	virtual void Render();
};