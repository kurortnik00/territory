#include "map.hpp"
#include "player.hpp"

Map::Map(float width, float height, int size, Player & left, Player & right)
	: width_ (width), height_ (height), size_ (size)
	, n_rows (width_ / size_), n_cols (height_ / size_)
	, left_(left), right_ (right)
	, map_ (n_rows, std::vector<sf::Sprite>(n_cols))
	, enabled_ (n_rows, std::vector<int>(n_cols))
{
	
}

int Map::size()
{
	return size_;
}

void Map::setTexture(sf::Texture & texture)
{
	for (int i = 0; i < n_rows; i++)
	{
		for (int j = 0; j < n_cols; j++)
		{
			map_[i][j].setTexture(texture);
			map_[i][j].setPosition(i * size_, j * size_);
			map_[i][j].setScale(static_cast<float>(size_) / texture.getSize().x, 
								static_cast<float>(size_) / texture.getSize().y);
		}
	}
}

void Map::update(BodyTracker & kinect, bool kinectControl)
{
	if (kinectControl)
	{
		auto& bodyMask = kinect.getBodyMask();
		for (int i = 0; i < bodyMask.size(); i++)
		{
			for (int j = 0; j < bodyMask[i].size(); j++)
			{
				if (bodyMask[i][j] != 255)
				{
					sf::Vector2i pos = getCell(kinect.GetProjection(sf::Vector2f(i, j)));
					bool is_left = (bodyMask[i][j] == 1);
					setEnabled(pos.x, pos.y, is_left);
				}
			}
		}
	}
}

void Map::setEnabled(int i, int j, bool left)
{
	if ((i >= 0) && (i < n_rows) && (j >= 0) && (j < n_cols))
	{
		if (left)
		{
			// Update score
			if (enabled_[i][j] == 2)
			{
				right_.lostCell();
			}
			if (enabled_[i][j] != 1)
			{
				left_.capturedCell();
			}

			// Update map
			enabled_[i][j] = 1;
			map_[i][j].setColor(Config::red);
		}
		else
		{
			// Update score
			if (enabled_[i][j] == 1)
			{
				left_.lostCell();
			}
			if (enabled_[i][j] != 2)
			{
				right_.capturedCell();
			}

			// Update map
			enabled_[i][j] = 2;
			map_[i][j].setColor(Config::green);
		}
	}
}

void Map::render(sf::RenderWindow& window)
{
	for (int i = 0; i < n_rows; i++)
	{
		for (int j = 0; j < n_cols; j++)
		{
			if (enabled_[i][j] > 0)
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