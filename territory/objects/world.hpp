#pragma once

#include <SFML/Graphics.hpp>
#include "../control/kinect/body_tracker.h"
#include "board.hpp"
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
    bool score_changed, paused, kinectControl, use_paddle_velocity;
    sf::RenderWindow mWindow;
    sf::Vector2f puck_velocity;
    Player left, right;
    Scoreboard board;
    ReadyButton left_ready, right_ready;
    sf::RectangleShape left_border, top_border, right_border;
};
