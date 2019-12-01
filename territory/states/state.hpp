#pragma once

#include "../objects/world.hpp"

namespace States
{
    enum Type
    {
        Preparation, Game, Result
    };
};

struct State
{
    State(States::Type type, World & world) : type(type), world (world) { }
    virtual ~State() { }

    virtual void processEvents() { }
    virtual void render() = 0;
    virtual States::Type switchTo() = 0;

    virtual void update(const float delta)
    {
        timeElapsed += delta;
    }

    virtual void reset()
    {
        timeElapsed = 0.f;
    }

    void log()
    {
        std::cout << type << " " << timeElapsed << "\n";
    }

protected:
    float timeElapsed = 0.f;
    States::Type type;
    World & world;
};

struct StatePreparation : public State
{
    StatePreparation(States::Type type, World & world) : State (type, world) { }

    ~StatePreparation() { }
    void processEvents() override;
    void update(const float delta) override;
    void render() override;
    States::Type switchTo() override;
};

struct StateGame : public State
{
    StateGame(States::Type type, World & world) : State (type, world) { }

    ~StateGame() { }
    void processEvents() override;
    void update(const float delta) override;
    void render() override;
    void reset() override;
    States::Type switchTo() override;

    unsigned max_score = 5;
};

struct StateResult : public State
{
    StateResult(States::Type type, World & world) : State (type, world) { }

    ~StateResult() { }
    void render() override;
    States::Type switchTo() override;
};
