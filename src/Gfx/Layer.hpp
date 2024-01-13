#pragma once

#include <string>

#include <Core/Config.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Layer
{
public:
    Layer()
        : m_Name("Unnamed layer"), m_Active(true) {}
    Layer(const std::string &name)
        : m_Name(name), m_Active(true) {}

    virtual ~Layer() = default;

    virtual bool Init(IGame &game) = 0;
    virtual bool HandleEvent(sf::Event &event, IGame &game) = 0;
    virtual void Render(IGame &game) = 0;

    std::string Name() const { return m_Name; }

    void SetActive(bool active) { m_Active = active; };
    void Toggle() { m_Active = !m_Active; }
    bool Active() const { return m_Active; }

private:
    std::string m_Name;
    bool m_Active;
};
