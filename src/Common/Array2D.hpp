#pragma once

#include <vector>

#include <Common/Types.hpp>

template<typename T>
class Array2D
{
public:
    Array2D(u32 x, u32 y)
        : m_Values({}), m_Size(std::make_pair(x, y)) {}

    Array2D(u32 x, u32 y, std::vector<T> initialState)
        : m_Values(initialState), m_Size(std::make_pair(x, y)) {}

    T& At(u32 x, u32 y)
    {
        auto [sizeX, sizeY] = m_Size;
        return m_Values[x * sizeX + y];
    }

    std::vector<T> GetPlain() const { return m_Values; }
    std::pair<u32, u32> Size() const { return m_Size; }

private:
    std::pair<u32, u32> m_Size;
    std::vector<T> m_Values;
};
