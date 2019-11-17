#include <iostream>

#include "state.hpp"

void StateResult::render()
{
    world.mWindow.clear();

    world.board.render(world.mWindow);

    world.mWindow.display();
}

States::Type StateResult::switchTo()
{
    if (timeElapsed > 5.f)
    {
        return States::Type::Preparation;
    }
    return States::Type::Result;
}
