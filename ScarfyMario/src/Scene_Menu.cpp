#include "Scene_Menu.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include "Scene_Play.h"

Scene_Menu::Scene_Menu(sa::GameEngine* gameEngine)
	:sa::Scene(gameEngine)
{
	init();
}

void Scene_Menu::init()
{
	registerAction(sf::Keyboard::W,      "UP"  );
	registerAction(sf::Keyboard::S,      "DOWN");
	registerAction(sf::Keyboard::Enter,  "NEXT");
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::M,      "MENU");

	m_title = "Scarfy Mario";

	m_menuStrings.push_back("Level 1");
	m_menuStrings.push_back("Level 2");

	m_levelPaths.push_back("../resources/level1.txt");
	m_levelPaths.push_back("../resources/level2.txt");

	
	m_menuText.setFont(m_gameEngine->getAssets().getFont("megaman"));
	m_menuText.setCharacterSize(64);

}

void Scene_Menu::update()
{
	m_entityManager.update();
}

void Scene_Menu::sRender()
{
	m_gameEngine->getWindow().clear(sf::Color(229, 37, 33));//100 100 255

	m_menuText.setCharacterSize(48);
	m_menuText.setString(m_title);
	m_menuText.setFillColor(sf::Color::Black);
	m_menuText.setPosition(sf::Vector2f(400, 10));
	m_gameEngine->getWindow().draw(m_menuText);

	for (size_t i = 0; i < m_menuStrings.size(); i++)
	{
		m_menuText.setString(m_menuStrings[i]);
		m_menuText.setFillColor(i == m_menuIndex ? sf::Color::White : sf::Color(0, 0, 0));
		m_menuText.setPosition(sf::Vector2f(10, 110 + i * 72)); // * 2.5f TODO
		m_gameEngine->getWindow().draw(m_menuText);
	}

	m_menuText.setCharacterSize(20);
	m_menuText.setFillColor(sf::Color::Black);
	m_menuText.setString("up: w    down: s    play: enter    menu: m      quit: esc");
	m_menuText.setPosition(sf::Vector2f(10, 690));
	m_gameEngine->getWindow().draw(m_menuText);

	//ingame-  pause: p  quit: esc  menu: m  move: a/d  jump: space  shoot:shift
	m_menuText.setCharacterSize(15);
	m_menuText.setFillColor(sf::Color::Black);
	m_menuText.setString("ingame-  pause: p    level select: esc    menu: m    move: a/d    jump: space    shoot:shift");
	m_menuText.setPosition(sf::Vector2f(10, 740));
	m_gameEngine->getWindow().draw(m_menuText);
}

void Scene_Menu::sDoAction(const sa::Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "UP")
		{
			if (m_menuIndex > 0) { m_menuIndex--; }
			else { m_menuIndex = m_menuStrings.size() - 1; }
		}

		else if (action.name() == "DOWN")
		{
			m_menuIndex = (m_menuIndex + 1) % m_menuStrings.size();
		}

		else if (action.name() == "NEXT")
		{
			m_gameEngine->changeScene("MARIO_GAME", std::make_shared<Scene_Play>(m_gameEngine, m_levelPaths[m_menuIndex]));
		}

		else if (action.name() == "MENU")
		{
			m_gameEngine->changeScene("MENU", nullptr, true);
		}

		else if (action.name() == "QUIT")
		{
			onEnd();
		}
	}
}

void Scene_Menu::onEnd()
{
	m_hasEnded = true;
	m_gameEngine->quit();
}
