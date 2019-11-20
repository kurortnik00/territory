#include <SFML/Graphics.hpp>
#include <iostream>

#include "state.hpp"

void StatePreparation::processEvents()
{
    world.processEvents();
}

void StatePreparation::update(const float delta)
{
    State::update(delta);

    world.left.update();
    world.right.update();
    world.left_ready.update(world.left.paddles());
    world.right_ready.update(world.left.paddles());
}

void StatePreparation::render()
{
    world.mWindow.clear();

    for (int i = 0; i < world.left.n_limbs; i++)
    {
        world.mWindow.draw(world.left.paddles()[i].shape());
    }
    for (int i = 0; i < world.right.n_limbs; i++)
    {
        world.mWindow.draw(world.right.paddles()[i].shape());
    }
    world.mWindow.draw(world.left_ready.shape());
    world.mWindow.draw(world.right_ready.shape());

    world.mWindow.display();
}

States::Type StatePreparation::switchTo()
{
    if (world.left_ready.isActivated() && world.right_ready.isActivated())
    {
        return States::Type::Game;
    }
    return States::Type::Preparation;
}
