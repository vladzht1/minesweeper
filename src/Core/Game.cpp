#include <iostream>
#include <cassert>
#include <time.h>

#include <Core/Game.hpp>
#include <Gfx/Layers/UILayer.hpp>
#include <Gfx/Layers/FieldLayer.hpp>
#include <Gfx/Layers/SystemLayer.hpp>
#include <Gfx/Layers/BackgroundLayer.hpp>
#include <SFML/Window/Event.hpp>

Game::Game()
{
    m_Field = new Field(m_Size, m_Mines, FieldGenerator().GenerateEmptyField(m_Size));
    m_GameConfig = GameConfig(m_Size, m_Mines);
    m_LayerManager = LayerManager();

    m_Running = false;

    auto systemLayer = new SystemLayer();
    auto backgroundLayer = new BackgroundLayer();
    auto fieldLayer = new FieldLayer();
    auto uiLayer = new UILayer();

    m_LayerManager.AddLayer(systemLayer);
    m_LayerManager.AddLayer(backgroundLayer);
    m_LayerManager.AddLayer(fieldLayer);
    m_LayerManager.AddLayer(uiLayer);
}

Game::~Game()
{
    delete m_Field;
}

void Game::Run()
{
    m_Window.create(
        sf::VideoMode(900, 950),
        "Minesweeper",
        sf::Style::Close
    );

    m_Window.setFramerateLimit(60);

    assert(m_LayerManager.InitLayers(*this));

    m_Running = true;

    // This is required for the game field random generation
    srand(time(NULL));

    GameLoop();
}

void Game::GameLoop()
{
    while(m_Running)
    {
        sf::Event event;

        while(m_Window.pollEvent(event))
        {
            m_LayerManager.HandleEvent(event, *this);

            CheckVictory();
            m_Window.clear();

            m_LayerManager.Render(*this);
        }

        m_Window.display();
        m_Running = m_Window.isOpen();
    }
}

void Game::Restart()
{
    m_GameConfig = GameConfig(m_Size, m_Mines);

    delete m_Field;
    m_Field = new Field(m_Size, m_Mines, FieldGenerator().GenerateEmptyField(GetConfig().Size));
}

void Game::CheckVictory()
{
    if(m_GameConfig.State.Status() == GameStatus::WIN)
    {
        return;
    }

    for(Tile& tile : m_Field->GetTiles().GetPlain())
    {
        if(!tile.IsMine() && !tile.IsOpen())
        {
            return;
        }
    }

    std::cout << "WIN!\n";
    m_GameConfig.State.SetStatus(GameStatus::WIN);
}

void Game::GenerateField(u32 firstClickX, u32 firstClickY)
{
    m_Field->SetField(FieldGenerator().GenerateField(m_Size, m_Mines, {firstClickX, firstClickY}));
}
