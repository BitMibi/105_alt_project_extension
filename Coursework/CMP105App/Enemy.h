#pragma once
#include "Player.h"
class Enemy :
    public Player
{

public:
    Enemy();
    void playerCollision(Player& player, AudioManager& m_audio);
    void setSpawnPos(sf::Vector2f position) { m_spawnPos = position; setPosition(m_spawnPos); };
    void reset(); 

    //definitely not stealing stuff from player...
    const float GRAVITY = 50.0f;
    const float DRAG_FACTOR = 0.999f;
    const float AIR_DRAG_FACTOR = 0.99f;
    const float TURN_DRAG = 0.6f;
private:
    bool m_isGrounded;
    sf::Vector2f m_spawnPos;

};

