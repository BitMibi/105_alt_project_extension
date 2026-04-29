#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"
#include <iostream>
class Star :
    public GameObject
{
public:
    Star();
    void update(float dt) override;
    void setAchieved();

private:
    sf::Texture m_starTexture;
    Animation* m_currentAnim;
    Animation m_shine;
    Animation m_grey;


    
};

