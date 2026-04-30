#pragma once
#include "Scene.h"
#include "Framework/Collision.h"
#include "Framework/TileMap.h"
#include "Player.h"
#include "TimeManager.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
class LevelTemplate :
    public Scene
{
public:
    LevelTemplate(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);


    void setUpLevel(TileMap& tmRef, TileMap& bgRef, float& oneStar, float& twoStar, float& threeStar, std::string dataFileToLoad, std::string tileMapFile, std::string bgFile, std::string textureFile, std::string bgTexture);
    
    void updateCameraAndBackground(Player& playerRef, TileMap& bgRef);

private:

    sf::Texture m_tileTexture; 
    //sf::Text m_alertText; would like to read in, but maybe later
    //sf::Font m_font; 

    const sf::Vector2i WORLD_SIZE;
    const sf::Vector2i VIEW_SIZE;
};

