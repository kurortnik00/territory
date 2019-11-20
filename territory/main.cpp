#include <SFML/Main.hpp>

#include <iostream>

#include "easylogging/easylogging++.h"
#include "objects/world.hpp"
#include "states/state.hpp"
#include "states/manager.hpp"

INITIALIZE_EASYLOGGINGPP

int main()
{
    bool kinectControl = true;
    sf::Time update_time = sf::seconds(1.f / 30.f);

    BodyTracker kinect;
    World world(1920.f, 1080.f, update_time.asSeconds(), kinect, kinectControl);
    StateManager manager(States::Type::Game, world, kinect, kinectControl);

    sf::Clock clock;
    sf::Time elapsed = sf::Time::Zero;
    while (world.mWindow.isOpen())
    {
        manager.processEvents();
        elapsed += clock.restart();

        while (elapsed > update_time)
        {
            manager.processEvents();
            manager.update(update_time.asSeconds());
            elapsed -= update_time;
        }

        manager.render();
    }

    return EXIT_SUCCESS;
}
