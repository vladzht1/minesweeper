#pragma once

#include <Gfx/Layer.hpp>

class SystemLayer : public Layer
{
public:
    SystemLayer()
        : Layer("SystemLayer") {}

    ~SystemLayer();

    bool Init(IGame &game) override;
    bool HandleEvent(sf::Event &event, IGame &game) override;
    void Render(IGame &game) override;
};
