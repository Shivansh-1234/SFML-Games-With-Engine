#pragma once

#include "Dependencies.h"

#include "Animation.h"

namespace sa {

	class Component
	{
	public:
		bool has = false;
	};

	class CTransform : public Component
	{
	public:
		Vec2 pos      = { 0.f, 0.f };
		Vec2 prevPos  = { 0.f, 0.f };
		Vec2 velocity = { 0.f, 0.f };
		Vec2 scale    = { 1.f, 1.f };
		float angle   = 0.f;

		CTransform() { }
		CTransform(const Vec2& p)
			:pos(p) { }
		CTransform(const Vec2& p, const Vec2& sp, const Vec2& sc, float a)
			:pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) { }
	};

	class CShape : public Component
	{
	public:
		sf::CircleShape circle;

		CShape(float radius, int points, const sf::Color& fillcolor, const sf::Color& outlineColor, float thickness)
			:circle(radius, points)
		{
			circle.setOrigin(radius, radius);
			circle.setFillColor(fillcolor);
			circle.setOutlineColor(outlineColor);
			circle.setOutlineThickness(thickness);
		}
	};

	//class CCollision
	//{
	//public:
	//	float radius = 0.f;

	//	CCollision(float r)
	//		:radius(r)
	//	{ }
	//};

	//class CScore
	//{
	//public:
	//	int score = 0;
	//	CScore(int s)
	//		:score(s)
	//	{ }
	//};

	class CLifespan : public Component
	{
	public:
		int lifeSpan     = 0;
		int frameCreated = 0;

		CLifespan() { }
		CLifespan(int duration, int frame)
			:lifeSpan(duration), frameCreated(frame) { }
	};

	class CInput : public Component
	{
	public:
		bool up       = false;
		bool down     = false;
		bool left     = false;
		bool right    = false;
		bool shoot    = false;
		bool canShoot = true;
		bool canJump  = true;

		CInput() { }
	};

	class CBoundingBox : public Component
	{
	public:
		Vec2 size;
		Vec2 halfSize;

		CBoundingBox() { }
		CBoundingBox(const Vec2& s)
			: size(s), halfSize(s.x / 2, s.y / 2) { }
	};

	class CGravity : public Component
	{
	public:
		float gravity = 0;
		CGravity() { }
		CGravity(float g)
			:gravity(g) { }
	};

	class CState : public Component
	{
	public:
		std::string state = "jumping";

		CState() { }
		CState(const std::string& s)
			:state(s) { }
	};

	class CAnimation : public Component
	{
	public:
		Animation animation;
		bool repeat = false;
		CAnimation() { }
		CAnimation(const Animation& anim, bool r)
			:animation(anim), repeat(r) { }
	};
}
