#include <Core/Field.hpp>

bool Tile::ToggleFlagged()
{
    if(IsOpen())
    {
        return false;
    }

    m_Flagged = !m_Flagged;
    return true;
}

OpenResult Tile::Open()
{
    if(IsOpen() || IsFlagged())
    {
        return OpenResult::NO_ACTION;
    }

    if(IsMine())
    {
        return OpenResult::LOSS;
    }

    m_Open = true;
    return OpenResult::SUCCESS;
}

Array2D<Tile> FieldGenerator::GenerateEmptyField(u32 size) const
{
    std::vector<Tile> tiles(size * size);

    for(i32 i = 0; i < size * size; i++)
    {
        tiles[i] = Tile();
    }

    return Array2D<Tile> (size, size, tiles);
}

Array2D<Tile> FieldGenerator::GenerateField(u32 size, u32 mines, sf::Vector2u firstClick) const
{
    Array2D<Tile> result = GenerateEmptyField(size);
    u32 minesSet = 0;

    while(minesSet != mines)
    {
        auto [x, y] = GetRandomPosition(size);

        if(result.At(x, y).IsMine() || (x == firstClick.x && y == firstClick.y))
        {
            continue;
        }

        result.At(x, y).SetMine();
        minesSet++;
    }

    return result;
}

std::pair<u32, u32> FieldGenerator::GetRandomPosition(u32 maxValue) const
{
    u32 randX = std::rand() % maxValue;
    u32 randY = std::rand() % maxValue;

    return std::make_pair(randX, randY);
}

OpenResult Field::Open(u32 x, u32 y)
{
    Tile& tileToOpen = m_Tiles.At(x, y);

    if(tileToOpen.IsFlagged())
    {
        return OpenResult::NO_ACTION;
    }

    if(tileToOpen.IsMine())
    {
        return OpenResult::LOSS;
    }

    std::vector<std::pair<u32, u32>> shouldOpen = {};
    shouldOpen.push_back(std::make_pair(x, y));

    while(shouldOpen.size() > 0)
    {
        auto [positionX, positionY] = shouldOpen.back();
        Tile& tile = m_Tiles.At(positionX, positionY);

        shouldOpen.pop_back();

        if(tile.IsOpen() || tile.IsFlagged() || tile.IsMine())
        {
            continue;
        }

        tile.Open();

        auto minesAround = GetMinesRound(positionX, positionY);

        if(minesAround > 0 && minesAround != GetFlaggedAround(positionX, positionY))
        {
            continue;
        }

        for(auto [offsetX, offsetY] : POSITIONS_AROUND)
        {
            i32 updatedPositionX = positionX + offsetX;
            i32 updatedPositionY = positionY + offsetY;

            if(!PositionValid(updatedPositionX, updatedPositionY))
            {
                continue;
            }

            shouldOpen.push_back(std::make_pair(positionX + offsetX, positionY + offsetY));
        }
    }

    return OpenResult::SUCCESS;
}

bool Field::ToggleFlagged(u32 x, u32 y)
{
    if(!PositionValid(x, y) || m_Tiles.At(x, y).IsOpen())
    {
        return false;
    }

    return m_Tiles.At(x, y).ToggleFlagged();
}

u32 Field::GetMinesRound(u32 x, u32 y)
{
    int minesAround = 0;

    for(sf::Vector2i offset : POSITIONS_AROUND)
    {
        i32 positionX = x + offset.x;
        i32 positionY = y + offset.y;

        if(!PositionValid(positionX, positionY))
        {
            continue;
        }

        if(m_Tiles.At(x + offset.x, y + offset.y).IsMine())
        {
            minesAround++;
        }
    }

    return minesAround;
}

// FIXME: Refactor duplication
u32 Field::GetFlaggedAround(u32 x, u32 y)
{
    int flaggedAround = 0;

    for(sf::Vector2i offset : POSITIONS_AROUND)
    {
        i32 positionX = x + offset.x;
        i32 positionY = y + offset.y;

        if(!PositionValid(positionX, positionY))
        {
            continue;
        }

        if(m_Tiles.At(x + offset.x, y + offset.y).IsFlagged())
        {
            flaggedAround++;
        }
    }

    return flaggedAround;
}

bool Field::PositionValid(i32 positionX, i32 positionY)
{
    return positionX >= 0 && positionX < m_Size && positionY >= 0 && positionY < m_Size;
}
