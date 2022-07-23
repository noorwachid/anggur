#pragma once

#include "Anggur/IO/Window.h"
#include "Anggur/Graphics/Renderer.h"

using namespace Anggur;

class Entity {
public:
	virtual void initialize() {};
	virtual void update(float deltaTime) {};
	virtual void terminate() {};

public:
	std::shared_ptr<Window> window;
	std::shared_ptr<Renderer> renderer;
};