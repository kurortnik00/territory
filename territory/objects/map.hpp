#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "../config/config.hpp"
#include "../control/kinect/body_tracker.h"
#include "player.hpp"

using MapContainer = std::vector<std::vector<sf::Sprite>>;
using FlagContainer = std::vector<std::vector<int>>;

struct Map
{
public:
	Map(float width, float height, int size, Player& left, Player& right);
	int size();
	void setTexture(sf::Texture& texture);
	void update(BodyTracker& kinect, bool kinectControl);
	void setEnabled(int i, int j, bool left);
	void render(sf::RenderWindow& window);

private:
	float width_, height_;
	int size_, n_rows, n_cols;
	Player & left_;
	Player & right_;
	MapContainer map_;
	FlagContainer enabled_;

	sf::Vector2i getCell(sf::Vector2f point);
};