#pragma once

#include <Gfx/Layer.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class BackgroundLayer : public Layer
{
public:
    BackgroundLayer()
        : Layer("BackgroundLayer") {}

    ~BackgroundLayer();

    bool Init(IGame &game) override;
    bool HandleEvent(sf::Event &event, IGame &game) override;
    void Render(IGame &game) override;

private:
    sf::RectangleShape* m_BackgroundRectangle;
};
