#include "Enemy.h"

Enemy::Enemy() {

	

	m_isGrounded = false;
}

void Enemy::playerCollision(Player& player, AudioManager& m_audio) {

	sf::FloatRect playerCollision = player.getCollisionBox();
	sf::FloatRect enemyCollision = getCollisionBox();

	auto overlap = enemyCollision.findIntersection(playerCollision);
	if (!overlap) return;
		
	//If Player collides with enemy, dies
	else {
		
		 
			player.reset();
			m_audio.playSoundbyName("death");
		


	}

}


void Enemy::reset() {
	setPosition(m_spawnPos);
	m_velocity = { 0,0 };

}