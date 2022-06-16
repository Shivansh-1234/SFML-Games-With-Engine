#include "Physics.h"

namespace sa {
	Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
	{
		if (!a->hasComponent<CBoundingBox>() || !b->hasComponent<CBoundingBox>())
			return Vec2(0, 0);

		Vec2 delta = { 
					   std::abs(a->getComponent<CTransform>().pos.x - b->getComponent<CTransform>().pos.x), 
					   std::abs(a->getComponent<CTransform>().pos.y - b->getComponent<CTransform>().pos.y) 
		             };

		Vec2 overlap =  a->getComponent<CBoundingBox>().halfSize + b->getComponent<CBoundingBox>().halfSize	;

		return Vec2(overlap.x - delta.x, overlap.y - delta.y);
	}

	Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
	{
		if (!a->hasComponent<CBoundingBox>() || !b->hasComponent<CBoundingBox>())
			return Vec2(0, 0);

		Vec2 delta = {
					   std::abs(a->getComponent<CTransform>().prevPos.x - b->getComponent<CTransform>().prevPos.x),
					   std::abs(a->getComponent<CTransform>().prevPos.y - b->getComponent<CTransform>().prevPos.y)
					 };

		Vec2 overlap = a->getComponent<CBoundingBox>().halfSize + b->getComponent<CBoundingBox>().halfSize;

		return Vec2(overlap.x - delta.x, overlap.y - delta.y);
	}


	bool Physics::checkCollision(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
	{
		Vec2 overlap = sa::Physics::GetOverlap(a, b);
		return (overlap.x > 0 && overlap.y > 0);
	}
}