#pragma once
#include "Scene.h"
#include "LevelTemplate.h"
#include "Framework/Collision.h"
#include "Framework/TileMap.h"
#include "Player.h"
#include "Coin.h"
#include "Flag.h"
#include <algorithm> // for clamp


class LevelTwoWithTiles :
    public LevelTemplate
{
public:
    LevelTwoWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);

    void handleInput(float dt) override;
    void update(float dt) override;
    void render() override;
    void onBegin() override;
    void onEnd() override;

private:
    void updateCameraAndBackground();
    void checkAndSetMessages();

    TileMap m_tilemap;
    TileMap m_bgtilemap;
    Player m_player;

    Coin m_coin;
    GameObject m_boopBlock;
    Flag m_flag;
    bool m_gameEnd = false;

    float m_oneStarTime;
    float m_twoStarTime;
    float m_threeStarTime;
    TimeManager m_timeManager;
    sf::Text m_timer;
    bool m_timerActive = false;

    const sf::Vector2i WORLD_SIZE = { 2880, 648 };
    const sf::Vector2i VIEW_SIZE = { 432, 432 };

    sf::Text m_alertText;
    sf::Font m_font;
    sf::Texture m_tileTexture;

    sf::Vector2f m_wallPos = { 11 * 72, 5 * 72 };


    std::vector<std::string> m_promptMessages =
    {
        "Oh I'm stuck.\nMaybe I missed\nsomething back there?",
        "I feel so powerful\nalmost like I\ncan double-jump",
        "Press F to end the day"
    };
};

