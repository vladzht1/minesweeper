#include <iostream>

#include <Gfx/Layers/UILayer.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Sprite.hpp>

UILayer::~UILayer()
{}

bool UILayer::Init(IGame &game)
{
    if(!m_Font.loadFromFile("../assets/fonts/mine-sweeper.ttf"))
    {
        std::cout << "Failed to load font\n";
        return false;
    }

    if(!m_FlagTexture.loadFromFile("../assets/images/flag.png"))
    {
        std::cout << "Failed to load flag image\n";
        return false;
    }

    m_UISize = std::abs((int)game.GetRenderWindow().getSize().y - (int)game.GetRenderWindow().getSize().x);
    m_LetterSize = m_UISize * 0.7f;

    m_MinesText.setFont(m_Font);
    m_MinesText.setCharacterSize(m_LetterSize);
    m_MinesText.setFillColor(sf::Color::Red);
    m_MinesText.setString(std::to_string(game.GetConfig().State.MinesLeft()));

    m_RestartButtonText.setFont(m_Font);
    m_RestartButtonText.setCharacterSize(m_LetterSize);
    m_RestartButtonText.setFillColor(sf::Color::Blue);
    m_RestartButtonText.setString("R");

    return true;
}

bool UILayer::HandleEvent(sf::Event &event, IGame &game)
{
    // TODO: Check for click in a different way
    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
    {
        f32 offset = 25.f;

        sf::Vector2f uiCenter = {game.GetRenderWindow().getSize().x * 0.5f, m_UISize * 0.5f};

        if(
            event.mouseButton.x > uiCenter.x - offset && event.mouseButton.x < uiCenter.x + offset &&
            event.mouseButton.y > uiCenter.y - offset && event.mouseButton.y < m_UISize
        )
        {
            game.Restart();
            return true;
        }
    }

    return false;
}

void UILayer::Render(IGame &game)
{
    sf::Sprite flagImage;

    m_MinesText.setString(std::to_string(game.GetConfig().State.MinesLeft()));
    m_MinesText.setPosition({
        flagImage.getGlobalBounds().width + 10.f,
        (m_UISize - m_MinesText.getLocalBounds().height) * 0.5f
    });

    m_RestartButtonText.setPosition({
        (game.GetRenderWindow().getSize().x - m_RestartButtonText.getLocalBounds().width) * 0.5f,
        (m_UISize - m_RestartButtonText.getLocalBounds().height) * 0.5f
    });

    flagImage.setTexture(m_FlagTexture);
    flagImage.setScale({
        m_UISize / m_FlagTexture.getSize().x,
        m_UISize / m_FlagTexture.getSize().y
    });
    flagImage.setPosition({
        m_MinesText.getGlobalBounds().width + 20.f,
        (m_UISize - flagImage.getGlobalBounds().height) * 0.5f
    });

    auto gameStatus = game.GetConfig().State.Status();

    if(gameStatus == GameStatus::WIN || gameStatus == GameStatus::LOST)
    {
        sf::Text statusText;


        statusText.setFont(m_Font);
        statusText.setString(gameStatus == GameStatus::WIN ? "WIN" : "LOSE");
        statusText.setFillColor(gameStatus == GameStatus::WIN ? sf::Color::Green : sf::Color::Red);
        statusText.setPosition(
            game.GetRenderWindow().getSize().x - statusText.getLocalBounds().width - 10.f,
            (m_UISize - statusText.getLocalBounds().height) * 0.5f
        );

        game.GetRenderWindow().draw(statusText);
    }

    game.GetRenderWindow().draw(m_MinesText);
    game.GetRenderWindow().draw(flagImage);
    game.GetRenderWindow().draw(m_RestartButtonText);
}
