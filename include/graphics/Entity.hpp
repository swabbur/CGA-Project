#pragma once

#include "Scene.hpp"

class Entity{

public:

	Scene const & scene;

	//Entity(Entity const&) = delete;

	Entity(Scene const & scene);

	~Entity();

};