#pragma once

#include <Core/Field.hpp>
#include <Common/Types.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

enum GameStatus
{
    IDLE,
    STARTED,
    WIN,
    LOST
};

class GameState
{
public:
    GameState()
        : m_MinesLeft(0) {}

    GameState(u32 mines)
        : m_MinesLeft(mines) {}

    GameStatus Status() const { return m_GameStatus; }
    u32 MinesLeft() const { return m_MinesLeft; }

    void IncreaseMines() { m_MinesLeft++; }
    void DecreaseMines() { m_MinesLeft--; }
    void SetStatus(GameStatus status) { m_GameStatus = status; }
    void SetMines(u32 mines) { m_MinesLeft = mines; }

private:
    GameStatus m_GameStatus = GameStatus::IDLE;
    i32 m_MinesLeft;
};

class GameConfig
{
public:
    GameConfig()
        : Size(16), Mines(40), State(GameState()) {}

    GameConfig(u32 size, u32 mines)
        : Size(size), Mines(mines), State(GameState()) {}

public:
    u32 Mines;
    u32 Size;
    GameState State;
};

class IGame
{
public:
    virtual ~IGame() {};

    virtual void Restart() = 0;

    virtual sf::RenderWindow& GetRenderWindow() = 0;
    virtual GameConfig& GetConfig() = 0;
    virtual void GenerateField(u32 firstClickX, u32 firstClickY) = 0;
    virtual Field* GetField() = 0;
};
