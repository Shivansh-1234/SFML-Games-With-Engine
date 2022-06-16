#include "EntityManager.h"

namespace sa {

	EntityManager::EntityManager()
	{
	}

	void EntityManager::removeDeadEntities(EntityVec& ev)
	{
		// This method gives dereference out of range vector iterator error . Still need to fix
		//auto it = ev.begin();

		//while (it != ev.end())
		//{
		//	if (!(*it)->isActive())
		//	{
		//		it = ev.erase(it);
		//		std::cout << "Erased Entity : " << (*it)->get_tag() << std::endl;
		//	}
		//	else
		//		it++;
		//}

		ev.erase(std::remove_if(ev.begin(), ev.end(), [](const std::shared_ptr<Entity>& entity) { return !entity->isActive(); }), ev.end());
	}

	void EntityManager::update()
	{

		for (auto e : m_entitiesToAdd)
		{
			m_entities.push_back(e);
			m_entityMap[e->get_tag()].push_back(e);

			std::cout << "Added an entity : " << e->get_tag() << std::endl;
		}

		m_entitiesToAdd.clear();

		removeDeadEntities(m_entities);

		for (auto& kv : m_entityMap)
		{
			removeDeadEntities(kv.second);
		}
	}

	std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
	{
		auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

		m_entitiesToAdd.push_back(entity); // Used to prevent iterator invalidation

		return entity;
	}
	const EntityVec& EntityManager::getEntities()
	{
		return  m_entities;
	}
	const EntityVec& EntityManager::getEntities(const std::string& tag)
	{
		return m_entityMap[tag];
	}
}
