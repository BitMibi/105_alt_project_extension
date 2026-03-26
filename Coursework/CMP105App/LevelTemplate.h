#pragma once
#include "Scene.h"
#include "Framework/Collision.h"
#include "Framework/TileMap.h"
#include "Player.h"
#include <algorithm>
class LevelTemplate :
    public Scene
{
public:
    LevelTemplate(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);


    void setUpLevel(std::string fileToLoad, std::string textureFile, std::string bgTexture);
    void tileMapReading(std::string& line, std::vector<int>& tileMap, int b);

private:
    
    void updateCameraAndBackground();

    TileMap m_tilemap; 
    TileMap m_bgtilemap; 
    Player m_player;  
    sf::Texture m_tileTexture; 
    sf::Text m_alertText; 
    sf::Font m_font; 

    const sf::Vector2i WORLD_SIZE;
    const sf::Vector2i VIEW_SIZE;
};

