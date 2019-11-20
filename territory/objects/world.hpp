#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "../control/kinect/body_tracker.h"
#include "board.hpp"
#include "map.hpp"
#include "player.hpp"
#include "ready_button.hpp"

struct World
{
public:
    World(float width, float height, float update_time, BodyTracker & kinect, bool kinectControl);
    void processEvents();

    void render();
    void reset();
    void update();

    int width_, height_;
    float update_time;
	BodyTracker& kinect;
    bool score_changed, paused, kinectControl, use_paddle_velocity;
    sf::RenderWindow mWindow;
	sf::Texture bodyTexture;
    Player left, right;
    Scoreboard board;
	Map map;
    ReadyButton left_ready, right_ready;
    sf::RectangleShape left_border, top_border, right_border;
};
