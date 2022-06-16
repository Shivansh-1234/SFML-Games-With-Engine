#pragma once

#include "DEPENDENCIES.h"
#include "Animation.h"

namespace sa {

	class Assets
	{
		std::map<std::string, sf::Texture> m_textureMap;
		std::map<std::string, sf::Font>    m_fontMap;
		std::map<std::string, Animation>   m_animationMap;
	public:
		Assets();

		void addTexture(const std::string& textureName, const std::string& path, bool smooth = true);
		void addFont(const std::string& fontName, const std::string& path);
		void addAnimation(const std::string& animationName, const std::string& textureName, size_t frameCount, size_t speed);

		const sf::Texture& getTexture(const std::string& textureName) const;
		const sf::Font& getFont(const std::string& fontName) const;
		const Animation& getAnimation(const std::string& animationName) const;

		void loadAssetsFromFile(const std::string& filePath);
	};

}

