#include "Animation.h"

namespace sa {

	Animation::Animation() { }

	Animation::Animation(const std::string& name, const sf::Texture& t)
		:Animation(name, t, 1, 0)  { }

	Animation::Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed)
		: m_name(name),
		  m_sprite(t),
		  m_frameCount(frameCount),
		  m_currentFrame(0),
		  m_speed(speed)           
	{ 
		m_size = Vec2((float)t.getSize().x / frameCount, (float)t.getSize().y);
		m_sprite.setOrigin(m_size.x / 2.f, m_size.y / 2.f);
		m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
	}

	void Animation::update()
	{
		m_currentFrame++;

		if (m_speed > 0)
		{
			int frame = (m_currentFrame / m_speed) % m_frameCount;

			m_sprite.setTextureRect(sf::IntRect(std::floor(frame) * m_size.x, 0, m_size.x, m_size.y));
		}

	}

	bool Animation::hasEnded() const
	{
		return m_currentFrame >= m_frameCount;
	}

	const std::string& Animation::getName() const
	{
		return m_name;
	}

	const Vec2& Animation::getSize() const
	{
		return m_size;
	}

	sf::Sprite& Animation::getSprite()
	{
		return m_sprite;
	}

}
