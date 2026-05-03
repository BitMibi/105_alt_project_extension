#pragma once
#include "Scene.h"
#include "LevelTemplate.h"
#include "Framework/Collision.h"
#include "Framework/TileMap.h"
#include "Player.h"
#include "Worm.h"
#include "Lever.h"
#include "Flag.h"
#include "TimeManager.h"
#include <algorithm>
#include <fstream>
class LevelThreeWithTiles :
    public LevelTemplate
{




   
    public:
        LevelThreeWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);

        void handleInput(float dt) override;
        void update(float dt) override;
        void render() override;
        void onBegin() override;
        void onEnd() override;

    private:
        void updateCameraAndBackground();

        TimeManager m_timeManager;
        TileMap m_tilemap;
        TileMap m_bgtilemap;
        Player m_player;
        std::vector<Worm*> m_wormList;
        sf::Texture m_tileTexture;
        Lever m_lever;
        sf::Text m_alertText;
        sf::Text m_timer;
        bool m_timerActive = false;
        sf::Font m_font;
        Flag m_flag;
        bool m_flagLeverPulled = false;
        float m_promptTimer;
        float m_oneStarTime;
        float m_twoStarTime;
        float m_threeStarTime;

        const float PROMPT_TIME = 2.f;
        const sf::Vector2i WORLD_SIZE = { 2880, 648 };
        const sf::Vector2i VIEW_SIZE = { 432, 432 };
    };



