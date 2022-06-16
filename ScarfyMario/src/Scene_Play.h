#pragma once

#include "DEPENDENCIES.h"

#include "Scene.h"

class Scene_Play : public sa::Scene
{

	struct PlayerConfig {
		float X, Y, CW, CH, SPEED, MAXSPEED, JUMP, GRAVITY;
		std::string WEAPON;
	};

protected:

	std::shared_ptr<sa::Entity> m_player;
	std::string                 m_levelPath;
	PlayerConfig                m_playerConfig;
	bool                        m_drawTextures = true;
	bool                        m_drawCollision = false;
	bool                        m_drawGrid = false;
	const sa::Vec2              m_gridSize = { 64, 64 };
	sf::Text                    m_gridText;
	sf::Text                    m_uiText;
	int                         m_score = 0;
	int                         m_health = 3;
	

	void init(const std::string& levelPath);
	void loadLevel(const std::string& fileName);
	void spawnPlayer();
	void spawnBullet(std::shared_ptr<sa::Entity> entity);
	void spawnCoin(std::shared_ptr<sa::Entity> entity);
	void drawline(const sa::Vec2& p1, const sa::Vec2& p2);

	sa::Vec2 gridToMidPixels(float gridX, float gridY, std::shared_ptr<sa::Entity> entity);

	//Systems
	void sAnimation();
	void sMovement();
	void sEnemySpawner(std::string& str, float x, float y);
	void sBulletSpawner();
	void sCollision();
	void sLifespan();
	void sDebug();
	void sUI();

	void update();
	void sRender();
	void sDoAction(const sa::Action& action);
	void onEnd();
public:
	Scene_Play(sa::GameEngine* gameEngine, const std::string& levelPath);
};

