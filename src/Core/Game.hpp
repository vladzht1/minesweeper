#pragma once

#include <Core/Config.hpp>
#include <Core/Field.hpp>
#include <Common/Types.hpp>
#include <Gfx/LayerManager.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Game : public IGame
{
public:
    Game();
    ~Game() override;

    void Run();
    void Restart() override;

    sf::RenderWindow& GetRenderWindow() override { return m_Window; }
    GameConfig& GetConfig() override { return m_GameConfig; }
    Field* GetField() override { return m_Field; }
    void GenerateField(u32 firstClickX, u32 firstClickY) override;

private:
    void GameLoop();
    void CheckVictory();

private:
    bool m_Running = false;
    sf::RenderWindow m_Window;

    LayerManager m_LayerManager;
    GameConfig m_GameConfig;
    Field* m_Field;

    u32 m_Mines = 40;
    u32 m_Size = 16;
};
