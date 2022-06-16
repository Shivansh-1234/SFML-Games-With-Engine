#pragma once

#include "DEPENDENCIES.h"

#include "EntityManager.h"
namespace sa {
	class Physics
	{
	public:
		static Vec2 GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
		static Vec2 GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);

		static bool checkCollision(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	};
}

