#include <iostream>

#include <Gfx/Layers/BackgroundLayer.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

bool BackgroundLayer::Init(IGame &game)
{
    auto [sizeX, sizeY] = game.GetRenderWindow().getSize();

    m_BackgroundRectangle = new sf::RectangleShape({(float)sizeX, (float)sizeY});
    m_BackgroundRectangle->setFillColor(sf::Color::White);

    return true;
}

bool BackgroundLayer::HandleEvent(sf::Event &event, IGame &game)
{
    // Handle NO events unless it's connected with the background
    return false;
}

void BackgroundLayer::Render(IGame &game)
{
    if(m_BackgroundRectangle == nullptr)
    {
        std::cout << "Failed to render background layer: class members are not initialized";
        std::exit(1);
    }

    game.GetRenderWindow().draw(*m_BackgroundRectangle);
}


BackgroundLayer::~BackgroundLayer()
{
    delete m_BackgroundRectangle;
}
