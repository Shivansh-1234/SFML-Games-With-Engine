#pragma once

#include "DEPENDENCIES.h"
#include "Scene.h"
#include "Assets.h"


namespace sa {

	typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

	class GameEngine
	{
	protected:
		sf::RenderWindow m_window;
		Assets           m_assets;
		std::string      m_currentScene;
		SceneMap         m_sceneMap;
		size_t           m_simulationSpeed = 1;
		bool             m_running = true;

		void init(const std::string& path);
		void update();

		void sUserInput();

	public:
		GameEngine(const std::string& path);

		void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

		void quit();

		void run();

		const Assets& getAssets() const;
		sf::RenderWindow& getWindow();
		std::shared_ptr<Scene> currentScene();
		bool isRunning();
	};

}

