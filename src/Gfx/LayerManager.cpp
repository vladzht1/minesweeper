#include <iostream>

#include <Gfx/LayerManager.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

/**
 * @brief Pushes a layer pointer to the end of the stack
 *
 * @param layer
 */
void LayerManager::AddLayer(Layer* layer)
{
    m_Layers.push_back(layer);
}

/**
 * @brief Removes (and deletes from the memory) the last layer off the stack
 *
 * @return true - if the layer exists and its removal was successful
 * @return false - overwise
 */
bool LayerManager::RemoveLayer()
{
    if(m_Layers.size() == 0)
    {
        return false;
    }

    Layer* lastLayer = m_Layers.back();
    m_Layers.pop_back();
    delete lastLayer;

    return true;
}

/**
 * @brief Removes (and deletes from the memory) the layer with the given name
 *
 * @param name
 * @return true - if the layer exists and its removal was successful
 * @return false
 */
bool LayerManager::RemoveLayer(const std::string &name)
{
    for(u32 i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->Name() == name)
        {
            Layer* layer = m_Layers[i];
            m_Layers.erase(m_Layers.begin() + i);
            delete layer;

            return true;
        }
    }

    return false;
}

/**
 * @brief Removes (and deletes from the memory) the layer by its index
 *
 * @param index
 * @return true - if layers exists and its removal was successful
 * @return false - overwise
 */
bool LayerManager::RemoveLayer(u32 index)
{
    if(index < 0 || index >= m_Layers.size())
    {
        return false;
    }

    Layer* layer = m_Layers[index];
    m_Layers.erase(m_Layers.begin() + index);
    delete layer;

    return true;
}

/**
 * @brief Returns a pointer to the layer with the given name with O(n) complexity.
 * If no layer was found, returns `nullptr`
 *
 * @param name
 * @return Layer*
 */
Layer* LayerManager::GetLayer(const std::string &name)
{
    for(auto layer : m_Layers)
    {
        if(layer->Name() == name)
        {
            return layer;
        }
    }

    return nullptr;
}

/**
 * @brief Returns a pointer to the layer at the given index in the stack.
 * If no layer was found, returns `nullptr`
 *
 * @param index
 * @return Layer*
 */
Layer* LayerManager::GetLayer(u32 index)
{
    if(index < 0 || index >= m_Layers.size())
    {
        return nullptr;
    }

    return m_Layers[index];
}

bool LayerManager::InitLayers(IGame &game)
{
    for(auto layer : m_Layers)
    {
        if(!layer->Init(game))
        {
            std::cout << "Failed to initialize layer " << layer->Name() << std::endl;
            return false;
        }
    }

    return true;
}

/**
 * @brief Emits event handler functions in all the layers in the stack `from LAST to FIRST`
 *
 * @param event
 * @return true - if event was handled by any layer
 * @return false - overwise
 */
bool LayerManager::HandleEvent(sf::Event &event, IGame &game)
{
    for(auto layer = m_Layers.rbegin(); layer != m_Layers.rend(); layer++)
    {
        if((*layer)->Active())
        {
            if((*layer)->HandleEvent(event, game))
            {
                return true;
            }
        }
    }

    return false;
}

/**
 * @brief Emits render functions in all the layers in the stack `from FIRST to LAST`
 *
 * @param window
 */
void LayerManager::Render(IGame &game)
{
    for(auto layer : m_Layers)
    {
        if(layer->Active())
        {
            layer->Render(game);
        }
    }
}
