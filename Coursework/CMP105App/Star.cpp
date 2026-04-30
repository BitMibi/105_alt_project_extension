#include "Star.h"
#include <fstream>


Star::Star() {
	if (!m_starTexture.loadFromFile("gfx/stars-sheet.png")) {
		std::cerr << "the stars have all died this is cruel and unusual;";
	}

	setTexture(&m_starTexture);

	for (int i = 0; i < 4; i++)
		m_shine.addFrame({ { i * 122, 0 }, { 122, 105} });

	m_shine.setLooping(true);
	

	m_grey.addFrame({ {0,105}, {122, 105} });
	m_grey.setLooping(true);
	
	
	m_shine.setFrameSpeed(0.4f);
	
	m_currentAnim = &m_grey;
}

void Star::update(float dt) {

	m_currentAnim->animate(dt);
	setTextureRect(m_currentAnim->getCurrentFrame());
}

void Star::setAchieved() {		//Function to set if the star has been achieved. If this is not called, the star remains grey and unachieved
	m_currentAnim = &m_shine;
}

void Star::setUnachieved() {
	m_currentAnim = &m_grey;
}