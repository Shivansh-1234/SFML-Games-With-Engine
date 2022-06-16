#pragma once

#include "DEPENDENCIES.h"

#include "Scene.h"


class Scene_Menu : public sa::Scene
{
protected:
	std::vector<std::string> m_menuStrings;
	std::vector<std::string> m_levelPaths;
	sf::Text                 m_menuText;
	std::string              m_title;
	int                      m_menuIndex;

	void init();

	void update();
	void sRender();
	void sDoAction(const sa::Action& action);
	void onEnd();
public:
	Scene_Menu(sa::GameEngine* gameEngine);
};

