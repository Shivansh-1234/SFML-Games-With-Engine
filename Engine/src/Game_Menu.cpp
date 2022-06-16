#include "Game_Menu.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include "../ScarfyMario/src/Scene_Menu.h"


Game_Menu::Game_Menu(sa::GameEngine* gameEngine)
	:sa::Scene(gameEngine)
{
	init();
}

void Game_Menu::init()
{
	registerAction(sf::Keyboard::W,      "UP"  );
	registerAction(sf::Keyboard::S,      "DOWN");
	registerAction(sf::Keyboard::Enter,  "NEXT");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_title = "MY GAMES";

	m_menuStrings.push_back("ScarfyMario");
	m_menuStrings.push_back("Zelda");
	m_menuStrings.push_back("MORE GAMES SOON !");

	
	m_menuText.setFont(m_gameEngine->getAssets().getFont("megaman"));
	m_menuText.setCharacterSize(64);

}

void Game_Menu::update()
{
	m_entityManager.update();
}

void Game_Menu::sRender()
{
	m_gameEngine->getWindow().clear(sf::Color(100, 100, 255));//100 100 255

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
	m_menuText.setString("up: w    down: s    play: enter       quit: esc");
	m_menuText.setPosition(sf::Vector2f(10, 690));
	m_gameEngine->getWindow().draw(m_menuText);
}

void Game_Menu::sDoAction(const sa::Action& action)
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
			switch (m_menuIndex)
			{
			case 0:
				m_gameEngine->changeScene("MARIO_MENU", std::make_shared<Scene_Menu>(m_gameEngine));
				break;
			default:
				break;
			}
			
		}

		else if (action.name() == "QUIT")
		{
			onEnd();
		}
	}
}

void Game_Menu::onEnd()
{
	m_hasEnded = true;
	m_gameEngine->quit();
}
