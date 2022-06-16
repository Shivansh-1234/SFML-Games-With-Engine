#include "Assets.h"
#include <cassert>
namespace sa {

	Assets::Assets(){ }

	void Assets::addTexture(const std::string& textureName, const std::string& path, bool smooth)
	{
		m_textureMap[textureName] = sf::Texture();

		if (!m_textureMap[textureName].loadFromFile(path))
		{
			std::cout << "Failed to load texture : " << path << std::endl;
			m_textureMap.erase(textureName);
		}

		else
		{
			m_textureMap[textureName].setSmooth(smooth);
			std::cout << "Loaded Texture : " << path << std::endl;
		}
	}

	void Assets::addFont(const std::string& fontName, const std::string& path)
	{
		m_fontMap[fontName] = sf::Font();

		if (!m_fontMap[fontName].loadFromFile(path))
		{
			std::cout << "Failed to load font : " << path << std::endl;
			m_fontMap.erase(fontName);
		}

		else
		{
			std::cout << "Loaded Font : " << path << std::endl;
		}
	}

	void Assets::addAnimation(const std::string& animationName, const std::string& textureName, size_t frameCount, size_t speed)
	{
		m_animationMap[animationName] = Animation(animationName, getTexture(textureName), frameCount, speed);
		std::cout << "Loaded Animation : " << animationName << " texture name : " << textureName << " framecount: " << frameCount << " speed: " << speed << std::endl;
	}

	const sf::Texture& sa::Assets::getTexture(const std::string& textureName) const
	{
		assert(m_textureMap.find(textureName) != m_textureMap.end());
		return m_textureMap.at(textureName);
	}

	const sf::Font& sa::Assets::getFont(const std::string& fontName) const
	{
		assert(m_fontMap.find(fontName) != m_fontMap.end());
		return m_fontMap.at(fontName);
	}
	const Animation& Assets::getAnimation(const std::string& animationName) const
	{
		assert(m_animationMap.find(animationName) != m_animationMap.end());
		return m_animationMap.at(animationName);
	}

	void Assets::loadAssetsFromFile(const std::string& filePath)
	{
		std::ifstream file(filePath);
		std::string str;

		while (file.good())
		{
			file >> str;

			if (str == "Texture")
			{
				std::string name, path;
				file >> name >> path;
				addTexture(name, path);
				//std::cout << "ASSET-> NAME: " << name << " PATH: " << path << std::endl;
			}

			else if (str == "Animation")
			{
				std::string name, texture;
				size_t frames, speed;
				file >> name >> texture >> frames >> speed;
				addAnimation(name, texture, frames, speed);
				//std::cout << "ANIM-> NAME : " << name << " TEXTURE: " << texture << " FRAME: " << frames << " SPEED: " << speed << std::endl;
			}

			else if (str == "Font")
			{
				std::string name, path;
				file >> name >> path;
				addFont(name, path);
				//std::cout << "FONT-> NAME: " << name << " PATH: " << path << std::endl;
			}

			else {
				std::cout << "FAILED TO LOAD ASSET : " << str << std::endl;
			}

		}
	}
}