#pragma once

#include "Anggur/IO/Window.h"
#include "Anggur/Graphics/Renderer.h"

using namespace Anggur;

class Entity {
public:
	Entity(Window& sharedWindow, Renderer& sharedRenderer): window(sharedWindow), renderer(sharedRenderer) {}

	virtual void initialize() {};
	virtual void update(float deltaTime) {};
	virtual void terminate() {};

public:
	Window& window;
	Renderer& renderer;
};