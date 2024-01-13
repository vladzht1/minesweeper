#pragma once

#include <Gfx/Layer.hpp>
#include <Common/Types.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

class UILayer : public Layer
{
public:
    UILayer()
        : Layer("UILayer") {}

    ~UILayer() override;

    bool Init(IGame &game) override;
    bool HandleEvent(sf::Event &event, IGame &game) override;
    void Render(IGame &game) override;

private:
    sf::Font m_Font;
    sf::Texture m_FlagTexture;
    sf::Text m_MinesText;
    sf::Text m_RestartButtonText;

    f32 m_UISize;
    f32 m_LetterSize;
};
