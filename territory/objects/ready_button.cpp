#include <SFML/Graphics.hpp>

#include "ready_button.hpp"

ReadyButton::ReadyButton(sf::Vector2f position, sf::Vector2f size)
    : position_ (position), size_ (size)
{
    shape_.setSize(size);
    shape_.setOrigin(size / 2.f);
    shape_.setPosition(position);
    shape_.setFillColor(sf::Color::Transparent);
    shape_.setOutlineThickness(5);
}


bool ReadyButton::isActivated()
{
    return activated;
}


sf::RectangleShape ReadyButton::shape()
{
    return shape_;
}


void ReadyButton::update(std::vector<Paddle> paddles)
{
    activated = false;
    for (int i = 0; i < static_cast<int>(paddles.size()); i++)
    {
        sf::Vector2f paddle_position = paddles[i].position();
        activated |= (paddle_position.x >= position_.x - size_.x / 2) &&
                     (paddle_position.x <= position_.x + size_.x / 2) &&
                     (paddle_position.y >= position_.y - size_.y / 2) &&
                     (paddle_position.y <= position_.y + size_.y / 2);
    }

    if (activated)
    {
        shape_.setOutlineColor(sf::Color::Green);
    }
    else {
        shape_.setOutlineColor(sf::Color::Red);
    }
}
