#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

#include "board.hpp"

using namespace std;

Scoreboard::Scoreboard(Player * left, Player * right, float gameDuration)
    : left_ (left), right_ (right), gameDuration_ (gameDuration)
{
    remainingTime_ = gameDuration_ * 60.f;
    font_.loadFromFile("DIN.ttf");

    float x = 120.f, y = 60.f;
    sf::Color red = sf::Color(204, 0, 0);
    sf::Color green = sf::Color(0, 102, 0);

    left_score_border.setPosition(400.f - x / 2 - y, 540.f);
    left_score_border.setSize(sf::Vector2f(y, 80.f));
    left_score_border.setFillColor(red);
    left_score_border.setOutlineColor(sf::Color::White);
    left_score_border.setOutlineThickness(-2.f);

    right_score_border.setPosition(400.f + x / 2, 540.f);
    right_score_border.setSize(sf::Vector2f(y, 80.f));
    right_score_border.setFillColor(green);
    right_score_border.setOutlineColor(sf::Color::White);
    right_score_border.setOutlineThickness(-2.f);

    main_border.setPosition(0.f, 540.f);
    main_border.setSize(sf::Vector2f(800.f, 80.f));
    main_border.setFillColor(sf::Color::Transparent);
    main_border.setOutlineColor(sf::Color::White);
    main_border.setOutlineThickness(-2.f);

    left_name.setFont(font_);
    left_name.setCharacterSize(40);
    left_name.setPosition(10.f, 545.f);
    left_name.setFillColor(sf::Color::White);
    left_name.setString("PLAYER 1");

    left_score.setFont(font_);
    left_score.setCharacterSize(40);
    left_score.setPosition(300.f, 545.f);
    left_score.setFillColor(sf::Color::White);
    left_score.setString(to_string(left_->score()));

    timer_.setFont(font_);
    timer_.setCharacterSize(40);
    timer_.setPosition(342.f, 545.f);
    timer_.setFillColor(sf::Color::White);
    timer_.setString(time_line(remainingTime_));

    right_score.setFont(font_);
    right_score.setCharacterSize(40);
    right_score.setPosition(480.f, 545.f);
    right_score.setFillColor(sf::Color::White);
    right_score.setString(to_string(right_->score()));

    right_name.setFont(font_);
    right_name.setCharacterSize(40);
    right_name.setPosition(610.f, 545.f);
    right_name.setFillColor(sf::Color::White);
    right_name.setString("PLAYER 2");
}

string Scoreboard::time_line(float seconds)
{
    int mins = (int) seconds / 60;
    int secs = (int) seconds % 60;
    sprintf(t_, "%02d:%02d", mins, secs);

    return t_;
}

void Scoreboard::update(float delta, bool & score_changed)
{
    if (score_changed)
    {
        left_score.setString(to_string(left_->score()));
        right_score.setString(to_string(right_->score()));
        score_changed = false;
    }
    remainingTime_ -= delta;
    timer_.setString(time_line(remainingTime_));
}

void Scoreboard::render(sf::RenderWindow & window)
{
    window.draw(left_score_border);
    window.draw(right_score_border);
    window.draw(main_border);

    window.draw(left_name);
    window.draw(left_score);
    window.draw(timer_);
    window.draw(right_score);
    window.draw(right_name);
}

void Scoreboard::reset()
{
    remainingTime_ = gameDuration_ * 60.f;
    left_score.setString(to_string(0));
    right_score.setString(to_string(0));
}
