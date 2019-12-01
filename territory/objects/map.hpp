#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "../control/kinect/body_tracker.h"

using MapContainer = std::vector<std::vector<sf::Sprite>>;
using FlagContainer = std::vector<std::vector<int>>;

struct Map
{
public:
	Map(float width, float height, int size);
	int size();
	void setTexture(sf::Texture& texture);
	void update(BodyTracker& kinect, bool kinectControl);
	void setEnabled(int i, int j);
	void render(sf::RenderWindow& window);

private:
	float width_, height_;
	int size_, n_rows, n_cols;
	MapContainer map_;
	FlagContainer enabled_;

	sf::Vector2i getCell(sf::Vector2f point);
};