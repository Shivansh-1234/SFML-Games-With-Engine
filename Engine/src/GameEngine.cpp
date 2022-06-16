#include "GameEngine.h"
#include "Game_Menu.h"

namespace sa {

	GameEngine::GameEngine(const std::string& path)
	{
		init(path);
	}

	void GameEngine::init(const std::string& path)
	{
		m_assets.loadAssetsFromFile(path);

		m_window.create(sf::VideoMode(1280, 768), "GameEngine By Shivy");
		m_window.setFramerateLimit(60);

		changeScene("MENU", std::make_shared<Game_Menu>(this), false);

	}
	void GameEngine::update()
	{
		if (!isRunning()) { return; }

		if (m_sceneMap.empty()) { return; }

		sUserInput();
		currentScene()->simulate(m_simulationSpeed);
		currentScene()->sRender();

		m_window.display();
	}
	void GameEngine::sUserInput()
	{
		sf::Event ev;
		while (m_window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed) quit();

			if (ev.type == sf::Event::KeyPressed || ev.type == sf::Event::KeyReleased)
			{
				if (currentScene()->getActionMap().find(ev.key.code) == currentScene()->getActionMap().end())
					continue;

				const std::string actionType = (ev.type == sf::Event::KeyPressed) ? "START" : "END";

				currentScene()->doAction(Action(currentScene()->getActionMap().at(ev.key.code), actionType));
			}
		}
	}
	std::shared_ptr<Scene> GameEngine::currentScene()
	{
		return m_sceneMap[m_currentScene];
	}
	
	void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
	{
		if (scene)
		{
			m_sceneMap[sceneName] = scene;
			std::cout << "Scene Changed to : " << sceneName << std::endl;
		}

		else
		{
			if (m_sceneMap.find(sceneName) == m_sceneMap.end())
			{
				std::cerr << "Warning : Scene does not exist : " << sceneName << std::endl;
				return;
			}
		}

		if (endCurrentScene)
		{
			m_sceneMap.erase(m_sceneMap.find(m_currentScene));
		}

		m_currentScene = sceneName;
	}

	void GameEngine::quit()
	{
		m_running = false;
	}

	void GameEngine::run()
	{
		while (isRunning())
		{
			update();
		}
	}

	const Assets& GameEngine::getAssets() const
	{
		return m_assets;
	}
	sf::RenderWindow& GameEngine::getWindow()
	{
		return m_window;
	}
	bool GameEngine::isRunning()
	{
		return m_running & m_window.isOpen();
	}
}