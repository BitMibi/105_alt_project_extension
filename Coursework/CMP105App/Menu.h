#pragma once
#include "Scene.h"
#include "Framework/Collision.h"
#include "Star.h"
#include <fstream>

class Menu :
    public Scene
{
public:
    Menu(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);

    void handleInput(float dt) override;
    void update(float dt) override;
    void render() override;
    void onBegin() override;
    void onEnd() override;
    void readSave();
    void newSave();

private:

    GameObject m_playButton;
    GameObject m_play2Button;
    GameObject m_resetButton;
    sf::Font m_font;
    sf::Text m_playButtonLabel;
    sf::Text m_playButton2Label;
    sf::Text m_resetButtonLabel;
    GameObject m_titleImage;
    sf::Texture m_titleSplash;
    std::array<Star*, 3 > m_level1Stars;
    std::array<Star*, 3> m_level2Stars;

    sf::Color m_defaultButtonColour = sf::Color(100, 250, 100, 50);
    sf::Color m_hoverButtonColour = sf::Color(80, 230, 80, 150);
    sf::Color m_lockedButtonColour = sf::Color(30, 30, 40, 200);

    //Checks for if a level is unlocked -- number corrosponds to locked level.
    bool m_locked2;
};

