#pragma once

#include "Dependencies.h"
#include "Components.h"

namespace sa {

	class EntityManager;

	typedef std::tuple<
		CTransform,
		CLifespan,
		CInput,
		CBoundingBox,
		CGravity,
		CState,
		CAnimation
	> ComponentTuple;

	class Entity
	{
		size_t         m_id     = 0;
		std::string    m_tag    = "default";
		bool           m_active = true;
		ComponentTuple m_components;

		Entity(const size_t id, const std::string& tag);

		friend class EntityManager;

	public:
		void  destroy();
		bool  isActive() const;
		const std::string& get_tag() const;
		const size_t get_id() const;

		template<typename T>
		T& getComponent()
		{
			return std::get<T>(m_components);
		}

		template<typename T>
		const T& getComponent() const
		{
			return std::get<T>(m_components);
		}

		template <typename T>
		bool hasComponent() const
		{
			return getComponent<T>().has;
		}

		template <typename T, typename... TArgs>
		T& addComponent(TArgs&&... mArgs)
		{
			auto& component = getComponent<T>();
			component = T(std::forward<TArgs>(mArgs)...);
			component.has = true;
			return component;
		}

		
	};
}
