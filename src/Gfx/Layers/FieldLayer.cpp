#include <cmath>
#include <iostream>

#include <Gfx/Layers/FieldLayer.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

FieldLayer::~FieldLayer() = default;

bool FieldLayer::Init(IGame &game)
{
    if(!m_FlagTexture.loadFromFile("../assets/images/flag.png"))
    {
        std::cout << "Failed to load flag image\n";
        return false;
    }

    if(!m_MineTexture.loadFromFile("../assets/images/mine.png"))
    {
        std::cout << "Failed to load mine image\n";
        return false;
    }

    if(!m_Font.loadFromFile("../assets/fonts/mine-sweeper.ttf"))
    {
        std::cout << "Failed to load font\n";
        return false;
    }

    auto windowSize = game.GetRenderWindow().getSize();

    m_TopOffset = std::abs((int)windowSize.x - (int)windowSize.y);
    m_TileSize = game.GetRenderWindow().getSize().x / (float)game.GetConfig().Size;

    return true;
}

bool FieldLayer::HandleEvent(sf::Event &event, IGame &game)
{
    if(game.GetConfig().State.Status() != GameStatus::IDLE && game.GetConfig().State.Status() != GameStatus::STARTED)
    {
        return false;
    }

    if(event.type == sf::Event::MouseButtonPressed)
    {
        u32 positionX = event.mouseButton.x / m_TileSize;
        u32 positionY = (event.mouseButton.y - m_TopOffset) / m_TileSize;

        if(event.mouseButton.button == sf::Mouse::Button::Left)
        {
            m_LastClick = { positionX, positionY };

            if(game.GetConfig().State.Status() == GameStatus::IDLE)
            {
                game.GenerateField(positionX, positionY);
                game.GetConfig().State.SetMines(game.GetConfig().Mines);
                game.GetConfig().State.SetStatus(GameStatus::STARTED);
            }

            OpenResult result = game.GetField()->Open(positionX, positionY);

            if(result == OpenResult::LOSS)
            {
                std::cout << "LOST!\n";
                game.GetConfig().State.SetStatus(GameStatus::LOST);

                return true;
            }

            return true;
        }

        if(event.mouseButton.button == sf::Mouse::Button::Right)
        {
            bool changed = game.GetField()->ToggleFlagged(positionX, positionY);

            if(game.GetField()->GetTile(positionX, positionY).IsFlagged() && changed)
            {
                game.GetConfig().State.DecreaseMines();
            }
            else if(changed)
            {
                game.GetConfig().State.IncreaseMines();
            }

            return true;
        }
    }

    return false;
}

void FieldLayer::Render(IGame &game)
{
    auto field = game.GetField()->GetTiles();
    auto [sizeX, sizeY] = field.Size();

    sf::RectangleShape sfTile;
    sf::Text sfLabel;

    for(int y = 0; y < sizeY; y++)
    {
        for(int x = 0; x < sizeX; x++)
        {
            Tile tile = field.At(x, y);

            sf::Vector2f currentPosition = {x * m_TileSize, y * m_TileSize + m_TopOffset};

            f32 outlineWidth = 3.f;

            sfTile.setSize({m_TileSize - outlineWidth, m_TileSize - outlineWidth});
            sfTile.setFillColor(sf::Color(198, 198, 198));
            sfTile.setOutlineColor(sf::Color(128, 128, 128));
            sfTile.setOutlineThickness(outlineWidth);
            sfTile.setPosition(currentPosition);

            if(tile.IsOpen())
            {
                u32 minesAround = game.GetField()->GetMinesRound(x, y);
                sfTile.setFillColor(sf::Color(192, 192, 192));

                if(minesAround > 0)
                {
                    sfLabel.setFont(m_Font);
                    sfLabel.setFillColor(TILE_LABEL_COLORS.at(minesAround));
                    sfLabel.setString(std::to_string(minesAround));
                    sfLabel.setCharacterSize(m_TileSize / 1.5);

                    f32 offsetX = (m_TileSize - sfLabel.getGlobalBounds().width) * 0.5;
                    f32 offsetY = (m_TileSize - sfLabel.getCharacterSize()) * 0.5;

                    sfLabel.setPosition({
                        currentPosition.x + offsetX,
                        currentPosition.y + offsetY
                    });
                }

                game.GetRenderWindow().draw(sfTile);
                game.GetRenderWindow().draw(sfLabel);
            }
            else if(game.GetConfig().State.Status() == GameStatus::LOST && tile.IsMine())
            {
                sf::Sprite mineImage;

                mineImage.setTexture(m_MineTexture);
                mineImage.setScale({
                    (m_TileSize / m_MineTexture.getSize().x) * 0.8f,
                    (m_TileSize / m_MineTexture.getSize().y) * 0.8f
                });

                f32 offset = (m_TileSize - mineImage.getGlobalBounds().width) * 0.5f;
                mineImage.setPosition({currentPosition.x + offset, currentPosition.y + offset});

                if(m_LastClick.has_value() && m_LastClick->x == x && m_LastClick->y == y)
                {
                    sfTile.setFillColor(sf::Color::Red);
                }

                game.GetRenderWindow().draw(sfTile);
                game.GetRenderWindow().draw(mineImage);
            }
            else
            {
                sf::ConvexShape lightBorder;
                sf::ConvexShape darkBorder;
                sf::RectangleShape centralSquare;

                lightBorder.setPointCount(3);
                darkBorder.setPointCount(3);

                lightBorder.setPoint(0, currentPosition);
                lightBorder.setPoint(1, {currentPosition.x + m_TileSize, currentPosition.y});
                lightBorder.setPoint(2, {currentPosition.x, currentPosition.y + m_TileSize});

                darkBorder.setPoint(0, {currentPosition.x, currentPosition.y + m_TileSize});
                darkBorder.setPoint(1, {currentPosition.x + m_TileSize, currentPosition.y});
                darkBorder.setPoint(2, {currentPosition.x + m_TileSize, currentPosition.y + m_TileSize});

                lightBorder.setFillColor(sf::Color::White);
                darkBorder.setFillColor(sf::Color(128, 128, 128));

                auto centralSquareSize = m_TileSize / 1.3f;
                auto offset = (m_TileSize - centralSquareSize) * 0.5f;

                centralSquare.setFillColor(sf::Color(198, 198, 198));
                centralSquare.setPosition({currentPosition.x + offset, currentPosition.y + offset});
                centralSquare.setSize({centralSquareSize, centralSquareSize});

                game.GetRenderWindow().draw(lightBorder);
                game.GetRenderWindow().draw(darkBorder);
                game.GetRenderWindow().draw(centralSquare);

                if(tile.IsFlagged())
                {
                    sf::Sprite flagSprite;

                    flagSprite.setTexture(m_FlagTexture);
                    flagSprite.setPosition({currentPosition.x + offset, currentPosition.y + offset});
                    flagSprite.setScale({
                        centralSquareSize / m_FlagTexture.getSize().x,
                        centralSquareSize / m_FlagTexture.getSize().y
                    });

                    game.GetRenderWindow().draw(flagSprite);
                }
            }
        }
    }
}

bool FieldLayer::MouseInFieldBounds(u32 x, u32 y, sf::RenderWindow &window)
{
    return x >= 0 && x < window.getSize().x && y >= 0 && y < window.getSize().y;
}
