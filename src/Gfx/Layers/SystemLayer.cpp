#include <Gfx/Layers/SystemLayer.hpp>
#include <SFML/Window/Event.hpp>

SystemLayer::~SystemLayer() = default;

bool SystemLayer::Init(IGame &game)
{
    return true;
}

bool SystemLayer::HandleEvent(sf::Event &event, IGame &game)
{
    if(event.type == sf::Event::Closed)
    {
        game.GetRenderWindow().close();
        return true;
    }

    return false;
}

void SystemLayer::Render(IGame &game)
{}
