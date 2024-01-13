#pragma once

#include <vector>
#include <iterator>

#include <Gfx/Layer.hpp>
#include <Common/Types.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class LayerManager
{
public:
    LayerManager()
        : m_Layers({}) {}

    Layer* GetLayer(u32 index);
    Layer* GetLayer(const std::string &name);

    void AddLayer(Layer* layer);
    bool RemoveLayer();
    bool RemoveLayer(const std::string &name);
    bool RemoveLayer(u32 index);

    bool InitLayers(IGame &game);
    bool HandleEvent(sf::Event &event, IGame &game);
    void Render(IGame &game);

private:
    std::vector<Layer*> m_Layers;
};
