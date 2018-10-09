#include "Renderable.h"

Renderable::Renderable() {
	Register(true);
}

void Renderable::Register(bool toRegister) {
	printf("Register: %d", toRegister);
}

void Renderable::Render() {}