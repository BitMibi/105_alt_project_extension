#pragma once
#include "Scene.h"
#include "LevelTemplate.h"
#include "Framework/Collision.h"
#include "Framework/TileMap.h"
#include "Player.h"
#include <algorithm>
#include <fstream>


class DebugLevel :
    public LevelTemplate
{
public:
    DebugLevel(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);

    void handleInput(float dt) override;
    void update(float dt) override;
    void render() override;
    void onBegin() override;
    void onEnd() override;

private:
    void updateCameraAndBackground();

    TileMap m_tilemap;
    TileMap m_bgtilemap;
    Player m_player;
    sf::Texture m_tileTexture;
    float m_oneStarTime;
    float m_twoStarTime;
    float m_threeStarTime;

    const float PROMPT_TIME = 2.f;
    const sf::Vector2i WORLD_SIZE = { 2880, 648 };
    const sf::Vector2i VIEW_SIZE = { 432, 432 };
};

