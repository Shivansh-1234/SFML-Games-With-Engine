#include "Scene.h"
#include "GameEngine.h"

namespace sa
{
    Scene::Scene() { }

    Scene::Scene(GameEngine* gameEngine)
        :m_gameEngine(gameEngine) { }

    void Scene::simulate(const int simSpeed)
    {
        for (int i = 0; i < simSpeed; i++)
        {
            update();
        }
    }
    void Scene::doAction(const Action& action)
    {
        sDoAction(action);
    }
    void Scene::registerAction(const int inputKey, const std::string& actionName)
    {
        this->m_actionMap[inputKey] = actionName;
    }

    void Scene::setPaused(bool paused)
    {
        m_paused = paused;
    }

    size_t Scene::getWindowWidth() const
    {
        return m_gameEngine->getWindow().getSize().x;
    }

    size_t Scene::getWindowHeight() const
    {
        return m_gameEngine->getWindow().getSize().y;
    }

    const size_t Scene::getCurrentFrame() const
    {
        return m_currentFrame;
    }
    ActionMap& Scene::getActionMap()
    {
        return m_actionMap;
    }
}