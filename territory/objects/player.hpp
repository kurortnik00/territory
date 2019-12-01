#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "../control/kinect/body_tracker.h"
#include "map.hpp"
#include "paddle.hpp"

struct Player
{
public:
    Player(float radius, sf::Color color, float update_time, BodyTracker & kinect, bool left, bool kinectControl);
    ~Player();
    void handleInput();
    void update(Map & map);
    std::vector<Paddle> & paddles();
    unsigned score();
    void scored();
    void reset();
    int n_limbs;

private:
    BodyTracker & kinect;
    bool left, kinectControl;
    std::vector<Paddle> paddles_;
    unsigned score_;
};
