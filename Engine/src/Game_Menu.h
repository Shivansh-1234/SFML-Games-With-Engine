#pragma once

#include "DEPENDENCIES.h"

#include "Scene.h"

class Game_Menu : public sa::Scene
{
protected:
	std::vector<std::string> m_menuStrings;
	sf::Text                 m_menuText;
	std::string              m_title;
	int                      m_menuIndex;

	void init();

	void update();
	void sRender();
	void sDoAction(const sa::Action& action);
	void onEnd();
public:
	Game_Menu(sa::GameEngine* gameEngine);
};


