#pragma once

#include <string>
#include "player.hpp"

struct Scoreboard
{
public:
    Scoreboard(Player & left, Player & right, float gameDuration);
    void update();
    void render(sf::RenderWindow & window);
    void reset();

private:
    std::string time_line(float seconds);
    char t_[8];
    Player & left_;
    Player & right_;
    sf::RectangleShape left_score_border, right_score_border, main_border;
    float remainingTime_, gameDuration_;
    sf::Font font_;
    sf::Text timer_, left_score, right_score, left_name, right_name;
};
