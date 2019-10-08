#pragma once
#ifndef SharedContext_hpp
#define SharedContext_hpp

#include "Input.hpp"
#include "WorkingDirectory.hpp"
#include "ResourceAllocator.hpp"
#include "Window.hpp"
#include "ImGuiLog.hpp"

class ObjectCollection;

struct SharedContext
{
	Input* input;
	ObjectCollection* objects;
	WorkingDirectory* workingDir;
	ResourceAllocator<sf::Texture>* textureAllocator;
	Window* window;
	ImGuiLog* imguilog;
};

#endif /* SharedContext_hpp */

