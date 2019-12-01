#pragma once

#include <unordered_map>

#include "../control/kinect/body_tracker.h"
#include "../objects/world.hpp"
#include "state.hpp"

using StateContainer = std::unordered_map<States::Type, State*>;

struct StateManager
{
    StateManager(const States::Type& initial, World & world, BodyTracker & kinect, bool kinectControl);
    ~StateManager();

    template <typename T>
    void registerState(const States::Type state, World & world);

    void activateState(const States::Type state);
    void processEvents();
    void update(const float delta);
    void render();

private:
    States::Type current_state;
    StateContainer container;
    World & world;
    BodyTracker & kinect;
    bool kinectControl;
};
