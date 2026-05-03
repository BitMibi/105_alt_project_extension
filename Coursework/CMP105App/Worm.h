#pragma once
#include "Enemy.h"
class Worm :
    public Enemy
{
public:
    Worm();
    void update(float dt) override;
    void collisionResponse(GameObject& collider) override;
    void playerCollision(Player& player, AudioManager& m_auido);
    void flip();
    

private:
    Animation m_walk;
    Animation* m_currAnim;
    sf::Vector2f m_accel;
    sf::Texture m_wormTexture;
   

    bool m_isGrounded;

    const float SPEED = 1.f;


};

