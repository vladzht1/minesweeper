#pragma once

#include <map>
#include <random>

#include <Common/Types.hpp>
#include <Common/Array2D.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

enum OpenResult
{
    LOSS,
    NO_ACTION,
    SUCCESS,
};

class Tile
{
public:
    Tile()
        : m_Open(false), m_Flagged(false), m_Mine(false) {}

    bool IsOpen() const { return m_Open; }
    bool IsMine() const { return m_Mine; }
    bool IsFlagged() const { return m_Flagged; }

    void SetMine() { m_Mine = true; }
    bool ToggleFlagged();

    OpenResult Open();

private:
    bool m_Open;
    bool m_Flagged;
    bool m_Mine;
};

class FieldGenerator
{
public:
    Array2D<Tile> GenerateEmptyField(u32 size) const;
    Array2D<Tile> GenerateField(u32 size, u32 mines, sf::Vector2u firstClick) const;

private:
    std::pair<u32, u32> GetRandomPosition(u32 maxValue) const;
};

class Field
{
public:
    Field(u32 size, u32 mines, Array2D<Tile> field)
        : m_Size(size), m_Tiles(field) {}

    u32 Size() const { return m_Size; }
    Tile& GetTile(u32 x, u32 y) { return m_Tiles.At(x, y); }
    Array2D<Tile> GetTiles() const { return m_Tiles; }

    void SetField(Array2D<Tile> field) { m_Tiles = field; }

    OpenResult Open(u32 x, u32 y);
    bool ToggleFlagged(u32 x, u32 y);

    u32 GetMinesRound(u32 x, u32 y);
    u32 GetFlaggedAround(u32 x, u32 y);

    bool PositionValid(i32 positionX, i32 positionY);

private:
    u32 m_Size;
    Array2D<Tile> m_Tiles;
};

const std::vector<sf::Vector2i> POSITIONS_AROUND = {
    sf::Vector2i(-1, 1),
    sf::Vector2i(0, 1),
    sf::Vector2i(1, 1),
    sf::Vector2i(-1, 0),
    sf::Vector2i(1, 0),
    sf::Vector2i(-1, -1),
    sf::Vector2i(0, -1),
    sf::Vector2i(1, -1),
};

const std::map<u32, sf::Color> TILE_LABEL_COLORS = {
    {0, sf::Color(255, 255, 255)},
    {1, sf::Color(80, 0, 255)},
    {2, sf::Color(1, 126, 0)},
    {3, sf::Color(255, 0, 0)},
    {4, sf::Color(40, 0, 128)},
    {5, sf::Color(144, 0, 0)},
    {6, sf::Color(2, 128, 127)},
    {7, sf::Color(0, 0, 0)},
    {8, sf::Color(128, 128, 128)},
};
