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
    , score_changed(false)
    , paused(false)
    , kinectControl (kinectControl)
    , use_paddle_velocity(false)
    , mWindow(sf::VideoMode(width, height), "Aerohockey", sf::Style::None)
    , left (height / 20, sf::Color(204, 0, 0), update_time, kinect, true, kinectControl)
    , right (height / 20, sf::Color(0, 102, 0), update_time, kinect, false, kinectControl)
    , board (&left, &right, 0.5)
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
    left.update();
    right.update();

    score_changed = false;
    board.update(update_time, score_changed);
}

void World::render()
{
    mWindow.clear();

    mWindow.draw(left_border);
    mWindow.draw(right_border);
    mWindow.draw(top_border);

    for (int i = 0; i < left.n_limbs; i++)
    {
        mWindow.draw(left.paddles()[i].shape());
    }
    for (int i = 0; i < right.n_limbs; i++)
    {
        mWindow.draw(right.paddles()[i].shape());
    }

    board.render(mWindow);

    mWindow.display();
}


void World::reset()
{
    left.reset();
    right.reset();
    board.reset();
}
