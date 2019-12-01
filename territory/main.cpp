#include <SFML/Main.hpp>

#include <iostream>

#include "easylogging/easylogging++.h"
#include "objects/world.hpp"
#include "states/state.hpp"
#include "states/manager.hpp"

INITIALIZE_EASYLOGGINGPP

int main()
{
    bool kinectControl = false;
    sf::Time update_time = sf::seconds(1.f / 30.f);

    BodyTracker kinect;
    World world(1024.f, 768.f, update_time.asSeconds(), kinect, kinectControl);
    StateManager manager(States::Type::Preparation, world, kinect, kinectControl);

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
