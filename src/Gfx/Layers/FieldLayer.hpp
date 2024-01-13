#pragma once

#include <optional>

#include <Gfx/Layer.hpp>
#include <Common/Types.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>

class FieldLayer : public Layer
{
public:
    FieldLayer()
        : Layer("FieldLayer") {}

    ~FieldLayer();

    bool Init(IGame &game) override;
    bool HandleEvent(sf::Event &event, IGame &game) override;
    void Render(IGame &game) override;

private:
    bool MouseInFieldBounds(u32 x, u32 y, sf::RenderWindow &window);

private:
    sf::Texture m_FlagTexture;
    sf::Texture m_MineTexture;
    sf::Font m_Font;

    std::optional<sf::Vector2u> m_LastClick;

    f32 m_TileSize;
    f32 m_TopOffset;
};
