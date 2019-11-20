#include "map.hpp"

Map::Map(float width, float height, int size)
	: width_ (width), height_ (height), size_ (size)
	, n_rows (height_ / size_), n_cols (width_ / size_)
	, map_ (n_rows, std::vector<sf::Sprite>(n_cols))
	, enabled_ (n_rows, std::vector<int>(n_cols))
{
	
}

void Map::setTexture(sf::Texture & texture)
{
	for (int i = 0; i < n_rows; i++)
	{
		for (int j = 0; j < n_cols; j++)
		{
			map_[i][j].setTexture(texture);
			map_[i][j].setPosition(j * size_, i * size_);
			map_[i][j].setScale(size_ / texture.getSize().x, size_ / texture.getSize().y);
			map_[i][j].setColor(sf::Color::Red);
		}
	}
}

void Map::update(BodyTracker & kinect)
{
	auto& bodyMask = kinect.getBodyMask();
	for (int i = 0; i < bodyMask.size(); i++)
	{
		for (int j = 0; j < bodyMask[i].size(); j++)
		{
			if (bodyMask[i][j] != 255)
			{
				sf::Vector2i pos = getCell(kinect.GetProjection(sf::Vector2f(i, j)));
				enabled_[pos.x][pos.y] = 1;
			}
		}
	}
}

void Map::render(sf::RenderWindow& window)
{
	for (int i = 0; i < n_rows; i++)
	{
		for (int j = 0; j < n_cols; j++)
		{
			if (enabled_[i][j])
			{
				window.draw(map_[i][j]);
			}
		}
	}
}

sf::Vector2i Map::getCell(sf::Vector2f point)
{
	return sf::Vector2i(point.x / size_, point.y / size_);
}