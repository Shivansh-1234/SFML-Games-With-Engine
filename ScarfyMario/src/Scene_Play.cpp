#include "Scene_Play.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include "Physics.h"

Scene_Play::Scene_Play(sa::GameEngine* gameEngine, const std::string& levelPath)
	:sa::Scene(gameEngine)
	,m_levelPath(levelPath)
{
	init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath)
{
	registerAction(sf::Keyboard::T,       "TOGGLE_TEXTURE");
	registerAction(sf::Keyboard::C,       "TOGGLE_COLLISION");
	registerAction(sf::Keyboard::G,       "TOGGLE_GRID");
										  
	registerAction(sf::Keyboard::P,       "PAUSE");
	registerAction(sf::Keyboard::Escape,  "QUIT");
	registerAction(sf::Keyboard::M,       "MENU");
										  
	registerAction(sf::Keyboard::D,       "MOVE_RIGHT");
	registerAction(sf::Keyboard::A,       "MOVE_LEFT");
	registerAction(sf::Keyboard::Space,   "JUMP");
	registerAction(sf::Keyboard::LShift,  "SHOOT");

	m_gridText.setCharacterSize(12);
	m_gridText.setFont(m_gameEngine->getAssets().getFont("lkf"));

	m_uiText.setCharacterSize(23);
	m_uiText.setFont(m_gameEngine->getAssets().getFont("megaman"));
	m_uiText.setFillColor(sf::Color::Yellow);

	loadLevel(levelPath);
}

void Scene_Play::loadLevel(const std::string& fileName)
{
	m_entityManager = sa::EntityManager();

	std::ifstream file(fileName);
	std::string str;
	float x, y;

	while (file.good())
	{
		file >> str;
		if (str == "Tile")
		{
			file >> str >> x >> y;
			auto tile = m_entityManager.addEntity("tile");
			tile->addComponent<sa::CAnimation>(m_gameEngine->getAssets().getAnimation(str), true);
			tile->addComponent<sa::CTransform>(gridToMidPixels(x, y, tile));
			tile->addComponent<sa::CBoundingBox>(m_gameEngine->getAssets().getAnimation(str).getSize());
		}

		if (str == "Enemy")
		{
			file >> str >> x >> y;
			sEnemySpawner(str, x, y);
		}

		if (str == "Decoration")
		{
			file >> str >> x >> y;
			auto decoration = m_entityManager.addEntity("decoration");
			decoration->addComponent<sa::CAnimation>(m_gameEngine->getAssets().getAnimation(str), true);
			decoration->addComponent<sa::CTransform>(gridToMidPixels(x, y, decoration));
		}

		if (str == "Player")
		{
			file >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CW >> m_playerConfig.CH >>
				 m_playerConfig.SPEED >> m_playerConfig.JUMP >> m_playerConfig.MAXSPEED >> m_playerConfig.GRAVITY >> m_playerConfig.WEAPON;

			/*std::cout << " " << m_playerConfig.X << " " << m_playerConfig.Y << " " << m_playerConfig.CW << " " << m_playerConfig.CH << " " << m_playerConfig.SPEED << " " << m_playerConfig.JUMP << " " << m_playerConfig.MAXSPEED << " " << m_playerConfig.GRAVITY << " " << m_playerConfig.WEAPON;*/
		}
	}

	spawnPlayer();

}

void Scene_Play::spawnPlayer()
{
	if (m_player) { m_player->destroy(); }

	m_player = m_entityManager.addEntity("player");
	m_player->addComponent<sa::CAnimation>(m_gameEngine->getAssets().getAnimation("AnimPlayer"), true);
	m_player->addComponent<sa::CTransform>(gridToMidPixels(m_playerConfig.X, m_playerConfig.Y, m_player));
	m_player->addComponent<sa::CBoundingBox>(sa::Vec2(m_playerConfig.CW, m_gameEngine->getAssets().getAnimation("AnimPlayer").getSize().y));
	m_player->addComponent<sa::CInput>();
	m_player->addComponent<sa::CGravity>(m_playerConfig.GRAVITY);
	m_player->addComponent<sa::CState>("air");
}

void Scene_Play::spawnBullet(std::shared_ptr<sa::Entity> entity)
{	
	auto& pTrans = entity->getComponent<sa::CTransform>();

	auto bullet = m_entityManager.addEntity("bullet");
	bullet->addComponent<sa::CAnimation>(m_gameEngine->getAssets().getAnimation(m_playerConfig.WEAPON), true);
	bullet->addComponent<sa::CTransform>(pTrans.pos, sa::Vec2(12 * pTrans.scale.x, 0), pTrans.scale, 0.f);
	bullet->addComponent<sa::CBoundingBox>(m_gameEngine->getAssets().getAnimation(m_playerConfig.WEAPON).getSize());
	bullet->addComponent<sa::CLifespan>(60, m_currentFrame);
}

void Scene_Play::spawnCoin(std::shared_ptr<sa::Entity> entity)
{
	auto coin = m_entityManager.addEntity("coin");
	coin->addComponent<sa::CTransform>(entity->getComponent<sa::CTransform>().pos + sa::Vec2(0, -64));
	coin->addComponent<sa::CAnimation>(m_gameEngine->getAssets().getAnimation("AnimCoin"), true);
	coin->addComponent<sa::CBoundingBox>(m_gameEngine->getAssets().getAnimation("AnimCoin").getSize());
	coin->addComponent<sa::CLifespan>(300, m_currentFrame);
}

void Scene_Play::drawline(const sa::Vec2& p1, const sa::Vec2& p2)
{
	sf::Vertex line[] = { sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y) };
	m_gameEngine->getWindow().draw(line, 2, sf::Lines);
}

sa::Vec2 Scene_Play::gridToMidPixels(float gridX, float gridY, std::shared_ptr<sa::Entity> entity)
{
	float lm = gridX * m_gridSize.x;
	float rm = getWindowHeight() - (gridY * m_gridSize.y);
	sa::Vec2 size = entity->getComponent<sa::CAnimation>().animation.getSize();

	return sa::Vec2(lm + size.x / 2, rm - size.y / 2);
}

void Scene_Play::sAnimation()
{
	
	auto& pState = m_player->getComponent<sa::CState>();
	auto& pTrans = m_player->getComponent<sa::CTransform>();

	if (pState.state == "stand")
	{
		auto& input = m_player->getComponent<sa::CInput>();

		if ((input.left || input.right) && !(input.left && input.right))
		{
			if (m_player->getComponent<sa::CAnimation>().animation.getName() != "AnimPlayerRun")
			{
				m_player->addComponent<sa::CAnimation>(m_gameEngine->getAssets().getAnimation("AnimPlayerRun"), true);
			}
		}

		else
		{
			m_player->addComponent<sa::CAnimation>(m_gameEngine->getAssets().getAnimation("AnimPlayer"), true);
		}
	}

	else if (pState.state == "air")
	{
		m_player->addComponent<sa::CAnimation>(m_gameEngine->getAssets().getAnimation("AnimPlayerJump"), true);
	}

	for (auto e : m_entityManager.getEntities())
	{
		if (!e->hasComponent<sa::CAnimation>()) { continue; }

		auto& cAnimation = e->getComponent<sa::CAnimation>();

		if (cAnimation.animation.hasEnded() && !cAnimation.repeat)
		{
			e->destroy();
		}

		else
		{
			e->getComponent<sa::CAnimation>().animation.update();
		}
	}
}

void Scene_Play::sMovement()
{
	auto& pInput = m_player->getComponent<sa::CInput>();
	auto& pTrans = m_player->getComponent<sa::CTransform>();

	sa::Vec2 playerInputSPeed = { 0.f, pTrans.velocity.y } ;

	if (pInput.right){
		pTrans.scale.x = 1;
		playerInputSPeed.x += m_playerConfig.SPEED;
	}

	if (pInput.left){
		pTrans.scale.x = -1;
		playerInputSPeed.x -= m_playerConfig.SPEED;
	}

	if (m_player->getComponent<sa::CInput>().up && m_player->getComponent<sa::CState>().state != "air")
	{
		playerInputSPeed.y = m_playerConfig.JUMP;
		m_player->getComponent<sa::CInput>().up = false;
		m_player->getComponent<sa::CInput>().canJump = true;
	}


	pTrans.velocity = playerInputSPeed;
	
	for (auto e : m_entityManager.getEntities())
	{
		auto& trans = e->getComponent<sa::CTransform>();

		if (e->hasComponent<sa::CGravity>())
		{
			trans.velocity.y += e->getComponent<sa::CGravity>().gravity;
		}

		trans.prevPos = trans.pos;
		trans.pos += trans.velocity;
	}

	if (m_player->getComponent<sa::CTransform>().velocity.y != 0)
	{
		m_player->getComponent<sa::CState>().state = "air";
	}
	
}

void Scene_Play::sEnemySpawner(std::string& str, float x, float y)
{
	auto enemy = m_entityManager.addEntity("enemy");
	enemy->addComponent<sa::CAnimation>(m_gameEngine->getAssets().getAnimation(str), true);
	enemy->addComponent<sa::CTransform>(gridToMidPixels(x, y, enemy), sa::Vec2(3, 0), sa::Vec2(1, 1), 0);
	enemy->addComponent<sa::CBoundingBox>(m_gameEngine->getAssets().getAnimation(str).getSize());
}

void Scene_Play::sBulletSpawner()
{
	auto& pInput = m_player->getComponent<sa::CInput>();
	if (pInput.shoot && pInput.canShoot)
	{
		spawnBullet(m_player);
		m_player->getComponent<sa::CInput>().canShoot = false;
	}
}

void Scene_Play::sCollision()
{
	//bullet and tile, bullet and enemy collision check
	for (auto b : m_entityManager.getEntities("bullet")) {
		for (auto t : m_entityManager.getEntities("tile")) {

			if (!t->hasComponent<sa::CBoundingBox>()) { continue; }

			if (sa::Physics::checkCollision(b, t) ) 
			{
				if (t->getComponent<sa::CAnimation>().animation.getName() == "AnimBrick") { b->destroy(); t->destroy(); }
				else { b->destroy(); }
			}
		}

		for (auto e : m_entityManager.getEntities("enemy"))
		{
			if (!e->hasComponent<sa::CBoundingBox>()) { continue; }

			if (sa::Physics::checkCollision(b, e))
			{
				e->destroy();
				b->destroy();
				m_score += 10;
			}
		}
	}

	//player and coin collision check
	for (auto coin : m_entityManager.getEntities("coin"))
	{
		if (sa::Physics::checkCollision(m_player, coin)) { coin->destroy(); m_score += 10; }
	}

	auto& pTrans = m_player->getComponent<sa::CTransform>();
	auto& pBox   = m_player->getComponent<sa::CBoundingBox>();

	//player an tile collision check
	for (auto tile : m_entityManager.getEntities("tile")) {

		if (!tile->hasComponent<sa::CBoundingBox>()) { continue; }

		if (tile->getComponent<sa::CAnimation>().animation.getName() == "AnimPole")
		{
			if (sa::Physics::checkCollision(m_player, tile))
			{
				onEnd();
			}
		}

		auto& tTrans = tile->getComponent<sa::CTransform>();

		sa::Vec2 overlap     = sa::Physics::GetOverlap(m_player, tile);
		sa::Vec2 prevOverlap = sa::Physics::GetPreviousOverlap(m_player, tile);

		if (overlap.x < 0 || overlap.y < 0) { continue; }

		sa::Vec2 diff = pTrans.pos - tTrans.pos;
		sa::Vec2 shift(0, 0);

		if (prevOverlap.x > 0)
		{
			shift.y += diff.y > 0 ? overlap.y : -overlap.y;
			pTrans.velocity.y = 0;

			if (diff.y < 0)
			{
				pTrans.pos += (tTrans.velocity);
				m_player->getComponent<sa::CState>().state = "stand";

				if (tile->getComponent<sa::CAnimation>().animation.getName() == "AnimEnemy")
				{
					tile->destroy();
				}
			}
			else
			{
				m_player->getComponent<sa::CState>().state = "air";

				if (tile->getComponent<sa::CAnimation>().animation.getName() == "AnimBrick")
				{
					tile->destroy();
				}

				if (tile->getComponent<sa::CAnimation>().animation.getName() == "AnimQuestiona")
				{
					tile->addComponent<sa::CAnimation>(m_gameEngine->getAssets().getAnimation("AnimQuestion"), true);
					spawnCoin(tile);
				}
			}
				
		}

		else if (prevOverlap.y > 0)
		{
			shift.x += diff.x > 0 ? overlap.x : -overlap.x;
			pTrans.velocity.x = 0;

			if (tile->getComponent<sa::CAnimation>().animation.getName() == "AnimEnemy")
			{
				spawnPlayer();
				m_health--;
			}
		}

		pTrans.pos += shift;
	}

	//player and enemy, enemy and tile collision check
	for (auto enemy : m_entityManager.getEntities("enemy")) {
		for (auto tile : m_entityManager.getEntities("tile")) {

			auto& eTrans = enemy->getComponent<sa::CTransform>();
			auto& tTrans = tile->getComponent<sa::CTransform>();
			auto& eBox   = enemy->getComponent<sa::CBoundingBox>();

			sa::Vec2 overlap = sa::Physics::GetOverlap(tile, enemy);
			sa::Vec2 prevOverlap = sa::Physics::GetPreviousOverlap(tile, enemy);

			if (overlap.x < 0 || overlap.y < 0) { continue; }

			sa::Vec2 diff = tTrans.pos - eTrans.pos;
			sa::Vec2 shift(0, 0);

			if (prevOverlap.x > 0)
			{
				shift.y += diff.y > 0 ? overlap.y : -overlap.y;
				eTrans.velocity.y = 0;

				if (diff.y < 0)
				{
					eTrans.pos += (tTrans.velocity);
				}
			}

			else if (prevOverlap.y > 0)
			{
				enemy->getComponent<sa::CTransform>().velocity.x *= -1;
			}
		}
		auto& eTrans = enemy->getComponent<sa::CTransform>();
		auto& eBox = enemy->getComponent<sa::CBoundingBox>();

		sa::Vec2 overlap = sa::Physics::GetOverlap(m_player, enemy);
		sa::Vec2 prevOverlap = sa::Physics::GetPreviousOverlap(m_player, enemy);

		if (overlap.x < 0 || overlap.y < 0) { continue; }

		sa::Vec2 diff = pTrans.pos - eTrans.pos;
		sa::Vec2 shift(0, 0);

		if (prevOverlap.x > 0)
		{
			if (diff.y < 0)
			{
				enemy->destroy();	
			}
		}

		else if (prevOverlap.y > 0)
		{
			spawnPlayer();
			m_health--;
		}

		if (eTrans.pos.y > getWindowHeight()) { enemy->destroy(); }
		if (eTrans.pos.x < eBox.halfSize.x) { eTrans.pos.x = eBox.halfSize.x; }
	}

	//player below ground collision
	if (pTrans.pos.y > getWindowHeight()) { spawnPlayer(); m_health--; }

	//player and left side of screen collision
	if (pTrans.pos.x < pBox.halfSize.x)   { pTrans.pos.x = pBox.halfSize.x; }
}

void Scene_Play::sLifespan()
{
	for (auto e : m_entityManager.getEntities())
	{
		if (e->hasComponent<sa::CLifespan>())
		{
			auto& lifespan = e->getComponent<sa::CLifespan>();
			int framesAlive = m_currentFrame - lifespan.frameCreated;

			if (framesAlive >= lifespan.lifeSpan)
			{
				e->destroy();
			}
		}
	}
}

void Scene_Play::sDebug()
{
	if (m_drawCollision)
	{
		for (auto e : m_entityManager.getEntities())
		{
			if (e->hasComponent<sa::CBoundingBox>())
			{
				auto& box = e->getComponent<sa::CBoundingBox>();
				auto& transform = e->getComponent<sa::CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
				rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
				rect.setPosition(transform.pos.x, transform.pos.y);
				rect.setFillColor(sf::Color(0, 0, 0, 0));
				rect.setOutlineColor(sf::Color(255, 255, 255, 255));
				rect.setOutlineThickness(1);
				m_gameEngine->getWindow().draw(rect);
			}
		}
	}

	if (m_drawGrid)
	{
		float leftX = m_gameEngine->getWindow().getView().getCenter().x - getWindowWidth() / 2;
		float rightX = leftX + getWindowWidth() + m_gridSize.x;
		float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

		for (float x = nextGridX; x < rightX; x += m_gridSize.x)
			drawline(sa::Vec2(x, 0), sa::Vec2(x, getWindowHeight()));

		for (float y = 0; y < getWindowHeight(); y += m_gridSize.y)
		{
			drawline(sa::Vec2(leftX, getWindowHeight() - y), sa::Vec2(rightX, getWindowHeight() - y));

			for (float x = nextGridX; x < rightX; x += m_gridSize.x)
			{
				std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
				std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
				m_gridText.setString("(" + xCell + "," + yCell + ")");
				m_gridText.setPosition(x + 3, getWindowHeight() - y - m_gridSize.y + 2);
				m_gameEngine->getWindow().draw(m_gridText);
			}
		}
	}
}

void Scene_Play::sUI()
{
	std::stringstream ss;
	ss << "Health: " << m_health << "        Score: " << m_score;
	m_uiText.setString(ss.str());

	if (m_health <= 0)
	{
		onEnd();
	}
}

void Scene_Play::sRender()
{
	if (!m_paused) { m_gameEngine->getWindow().clear(sf::Color(100, 100, 255)); }
	else { m_gameEngine->getWindow().clear(sf::Color(50, 50, 150)); }

	auto& pPos = m_player->getComponent<sa::CTransform>().pos;
	float windowCentreX = std::max(m_gameEngine->getWindow().getSize().x / 2.f, pPos.x);
	sf::View view = m_gameEngine->getWindow().getView();
	view.setCenter(windowCentreX, m_gameEngine->getWindow().getSize().y - view.getCenter().y);
	m_gameEngine->getWindow().setView(view);
	m_uiText.setPosition(m_gameEngine->getWindow().getView().getCenter().x - (getWindowWidth() / 2), m_gameEngine->getWindow().getView().getCenter().y - (getWindowHeight() / 2));

	if (m_drawTextures)
	{
		for (auto& e : m_entityManager.getEntities())
		{
			auto& transform = e->getComponent<sa::CTransform>();

			if (e->hasComponent<sa::CAnimation>())
			{
				auto animation = e->getComponent<sa::CAnimation>().animation;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				m_gameEngine->getWindow().draw(animation.getSprite());
			}
		}
	}

	if (m_levelPath == "../resources/level3.txt")
	{
		m_uiText.setPosition(getWindowWidth() / 4, getWindowHeight() / 4);
		m_uiText.setCharacterSize(50);
		m_uiText.setString("SOON . STAY TUNED!");
	}

	m_gameEngine->getWindow().draw(m_uiText);
	sDebug();

}

void Scene_Play::sDoAction(const sa::Action& action)
{
	if (action.type() == "START")
	{
			 if (action.name() == "TOGGLE_TEXTURE")   { m_drawTextures = !m_drawTextures; }
		else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
		else if (action.name() == "TOGGLE_GRID")      { m_drawGrid = !m_drawGrid; }

		else if (action.name() == "PAUSE")            { m_paused = !m_paused; }
		else if (action.name() == "QUIT")			  { onEnd(); }
		else if (action.name() == "MENU")             { m_gameEngine->changeScene("MENU", nullptr, true); }

		else if (action.name() == "MOVE_RIGHT")       { m_player->getComponent<sa::CInput>().right = true; }
		else if (action.name() == "MOVE_LEFT")        { m_player->getComponent<sa::CInput>().left = true; }
		else if (action.name() == "SHOOT")            { m_player->getComponent<sa::CInput>().shoot = true; }
		else if (action.name() == "JUMP")             { m_player->getComponent<sa::CInput>().up = true;}
	}

	else if (action.type() == "END")
	{
		     if (action.name() == "MOVE_RIGHT")       { m_player->getComponent<sa::CInput>().right = false; }
		else if (action.name() == "MOVE_LEFT")        { m_player->getComponent<sa::CInput>().left = false; }
		else if (action.name() == "SHOOT")            { m_player->getComponent<sa::CInput>().shoot = false; m_player->getComponent<sa::CInput>().canShoot = true; }
		else if (action.name() == "JUMP") {
			if (m_player->getComponent<sa::CTransform>().velocity.y < 0)
			{
				m_player->getComponent<sa::CTransform>().velocity.y = 0;
			}

			m_player->getComponent<sa::CInput>().up = false;
			m_player->getComponent<sa::CInput>().canJump = true;
		}
	}

}

void Scene_Play::update()
{
	m_entityManager.update();

	if (!m_paused)
	{
		sMovement();
		sCollision();
		sAnimation();
		sLifespan();
		sBulletSpawner();
		sUI();
	}

	m_currentFrame++;
}

void Scene_Play::onEnd()
{
	m_gameEngine->getWindow().setView(m_gameEngine->getWindow().getDefaultView());
	m_hasEnded = true;
	m_gameEngine->changeScene("MARIO_MENU", nullptr, true);
}
