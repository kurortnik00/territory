#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>

#include "world.hpp"
#include "../util.hpp"

using namespace std;


World::World(float width, float height, float update_time, BodyTracker & kinect, bool kinectControl)
    : width_(width)
    , height_(height)
    , update_time (update_time)
	, kinect (kinect)
    , score_changed(false)
    , paused(false)
    , kinectControl (kinectControl)
    , use_paddle_velocity(false)
    , mWindow(sf::VideoMode(width, height), "Aerohockey", sf::Style::None)
    , left (height / 20, sf::Color(204, 0, 0), update_time, kinect, true, kinectControl)
    , right (height / 20, sf::Color(0, 102, 0), update_time, kinect, false, kinectControl)
    , board (&left, &right, 0.5)
	, map (width, height, 20)
    , left_ready (sf::Vector2f(width / 4, height / 2), sf::Vector2f(width / 10, width / 10))
    , right_ready (sf::Vector2f(width * 3 / 4, height / 2), sf::Vector2f(width / 10, width / 10))
{
    mWindow.setFramerateLimit(60);
    mWindow.setVerticalSyncEnabled(true);

    left_border.setPosition(0.f, 0.f);
    left_border.setSize(sf::Vector2f(2.f, height_ - 60.f));
    left_border.setFillColor(sf::Color(204, 0, 0));

    right_border.setPosition(798.f, 0.f);
    right_border.setSize(sf::Vector2f(2.f, height_ - 60.f));
    right_border.setFillColor(sf::Color(0, 102, 0));

    left_border.setPosition(0.f, 0.f);
    left_border.setSize(sf::Vector2f(800.f, 2.f));
    left_border.setFillColor(sf::Color::White);

	if (!bodyTexture.loadFromFile("media/textures/tiles.jpg", sf::IntRect(0, 0, 97, 97)))
	{
		LOG(ERROR) << "Failed to create body texture";
	}
	map.setTexture(bodyTexture);
	bodySprite.setTexture(bodyTexture);
}


void World::processEvents()
{
    if (!kinectControl)
    {
        left.handleInput();
        right.handleInput();
    }
}


void World::update()
{
	map.update(kinect, kinectControl);
    left.update(map);
    right.update(map);

    /*score_changed = false;
    board.update(update_time, score_changed);*/
}

void World::render()
{
    mWindow.clear();

	map.render(mWindow);
	for (int i = 0; i < left.paddles().size(); i++)
	{
		mWindow.draw(left.paddles()[i].shape());
	}
	for (int i = 0; i < right.paddles().size(); i++)
	{
		mWindow.draw(right.paddles()[i].shape());
	}
	//mWindow.draw(bodySprite);

    mWindow.display();
}


void World::reset()
{
    left.reset();
    right.reset();
    board.reset();
}
