#include "Worm.h"


Worm::Worm() {
	if (!m_wormTexture.loadFromFile("gfx/worm_sheet.png")) {
		std::cerr << "WORMIAS IS DEAD";
	}
	setTexture(&m_wormTexture);
	for (int i = 0; i < 6; i++) {
		m_walk.addFrame({ {i * 64, 0}, { 64,64 } });
	}

	m_walk.setLooping(true);
	m_walk.setFrameSpeed(0.2f);

	setCollisionBox({ {12,12}, {64,64} });
	m_currAnim = &m_walk;

}

void Worm::update(float dt) {

	// newtonian model
	if (m_accel.y < 30.f) { //terminal velocity
		m_accel.y += GRAVITY;
	}
	if (m_velocity.x < 5.f && m_velocity.x > -5.f && m_isGrounded) { //Don't move too fast, and only when on the ground (worms can't fly!)
		if (m_currAnim->getFlipped()) {
			m_accel.x -= SPEED;		//auto move left
		}
		else {
			m_accel.x += SPEED;		//auto move right 
		}
	}
	m_velocity += dt * m_accel;
	
	if (m_isGrounded && abs(m_accel.x) < 1.f) m_velocity *= DRAG_FACTOR;
	else if (!m_isGrounded) m_velocity *= AIR_DRAG_FACTOR;
	else if (m_accel.x * m_velocity.x < 0) m_velocity *= TURN_DRAG;

	m_isGrounded = false;	// every frame it is falling unless proved otherwise by floor collision

	// handle animation
	float speed = std::abs(m_velocity.x);	// sideways speed
	

	// face direction
	if (m_velocity.x > 0 && m_currAnim->getFlipped()
		|| m_velocity.x < 0 && !m_currAnim->getFlipped())
		// if we gotta flip, flip.
		m_currAnim->setFlipped(!m_currAnim->getFlipped());

	move(m_velocity);


	m_currAnim->animate(dt);
	setTextureRect(m_currAnim->getCurrentFrame());



}

void Worm::collisionResponse(GameObject& collider)
{
	sf::FloatRect wormCollider = getCollisionBox();
	sf::FloatRect wallBounds = collider.getCollisionBox();



	auto overlap = wormCollider.findIntersection(wallBounds);
	if (!overlap) return; // if there is no overlap, then leave.
	if (overlap->size.x < overlap->size.y)
	{
		// taller than wide -> collision is side-on
		if (wormCollider.position.x < wallBounds.position.x){
			m_velocity.x = 0;
			move({ -overlap->size.x, 0 });
			m_currAnim->setFlipped(true);
		}
		else {
			m_velocity.x = 0;
			move({ overlap->size.x, 0 });
			m_currAnim->setFlipped(false);
		}
	}
	else
	{
		if (wormCollider.position.y < wallBounds.position.y)
		{
			// We are above the wall (Landing)
			move({ 0, -overlap->size.y });
			m_velocity.y = 0;       // Stop falling
			m_isGrounded = true;
		}
		else
		{
			// We hit the ceiling (Bonk)
			move({ 0, overlap->size.y });
			m_velocity.y = 0;       // Stop moving up
		}
	}
}

void Worm::playerCollision(Player& player, AudioManager& m_audio) {

	sf::FloatRect playerCollision = player.getCollisionBox();
	sf::FloatRect enemyCollision = getCollisionBox();

	auto overlap = enemyCollision.findIntersection(playerCollision);
	if (!overlap) return;
	if (overlap->size.x < overlap->size.y) {
		if (player.getPosition().y < getPosition().y) {
			
			player.setVelocity({ player.getVelocity().x, -15.f});
			setAlive(false);
		}
		else {
			player.reset();
			m_audio.playSoundbyName("death");
			//If Player collides with enemy, dies
		}
	}
	else {
		if (player.getPosition().y < getPosition().y) {
			player.setVelocity({ player.getVelocity().x, -15.f });
			setAlive(false);
		}
		else {
			player.reset();
			m_audio.playSoundbyName("death");
		}


	}

}

void Worm::flip() {
	if (m_currAnim->getFlipped()) { m_currAnim->setFlipped(false); }
	else { m_currAnim->setFlipped(true); }
}