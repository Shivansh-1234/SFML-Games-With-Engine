#pragma once

#include "DEPENDENCIES.h"

#include "EntityManager.h"
#include "Action.h"

namespace sa
{
	class GameEngine;

	typedef std::map<int, std::string> ActionMap;

	class Scene 
	{
	protected:
		GameEngine*   m_gameEngine = nullptr;
		EntityManager m_entityManager;
		ActionMap     m_actionMap;
		size_t        m_currentFrame = 0;
		bool          m_paused = false;
		bool          m_hasEnded = false;

		virtual void onEnd() = 0;
		void setPaused(bool paused);

	public:

		Scene();
		Scene(GameEngine* gameEngine);

		virtual void update()    = 0;
	    virtual void sDoAction(const Action& action) = 0;
		virtual void sRender()   = 0;

		void simulate(const int simSpeed);
		void doAction(const Action &action);
		void registerAction(const int inputKey, const std::string& actionName);

		size_t getWindowWidth() const;
		size_t getWindowHeight() const;
		const size_t getCurrentFrame() const;
		ActionMap& getActionMap();


	};

}