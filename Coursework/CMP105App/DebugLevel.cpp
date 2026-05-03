#include "DebugLevel.h"


DebugLevel::DebugLevel(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio)
	: LevelTemplate(window, input, gameState, audio)
{

	setUpLevel(m_tilemap, m_bgtilemap, m_oneStarTime, m_twoStarTime, m_threeStarTime, "data/debuglevel.txt", "data/debugLevelTM.txt", "data/debugLevelBG.txt", "gfx/tilemap.png", "gfx/tilemap-backgrounds.png");
	m_tilemap.buildLevel();
	m_bgtilemap.buildLevel();

	m_player.setAudio(&m_audio);
	m_player.setInput(&m_input);
	m_player.setEdges(0, WORLD_SIZE.x);
	m_player.setCanDoubleJump(true);
	m_player.setEndGamePosition({ 864, 325 });


	m_wormias.setSpawnPos({ 520,50 });
}

void DebugLevel::handleInput(float dt)
{
	m_player.handleInput(dt);

	if (m_input.isPressed(sf::Keyboard::Scancode::Escape))
		m_gameState.setCurrentState(State::MENU);
}


void DebugLevel::update(float dt) {
	m_player.update(dt);
	m_wormias.update(dt);


	std::vector<GameObject>& level = *m_tilemap.getLevel();
	for (auto& t : level)
	{
		if (t.isCollider() && Collision::checkBoundingBox(m_player, t))
		{
			m_player.collisionResponse(t);
		}
		if (t.isCollider() && Collision::checkBoundingBox(m_wormias, t)) {
			m_wormias.collisionResponse(t);
		}
	}

	if (m_wormias.isAlive()) {
		m_wormias.playerCollision(m_player, m_audio);
	}
	

	updateCameraAndBackground();
}

void DebugLevel::updateCameraAndBackground()
{
	auto view = m_window.getView();
	auto player_pos = m_player.getPosition() + m_player.getSize() * 0.5f;

	float halfViewWidth = VIEW_SIZE.x / 2.0f;
	float halfViewHeight = VIEW_SIZE.y / 2.0f;

	player_pos.x = std::clamp(player_pos.x, halfViewWidth, WORLD_SIZE.x - halfViewWidth);
	player_pos.y = std::clamp(player_pos.y, halfViewHeight, WORLD_SIZE.y - halfViewHeight);

	view.setCenter(player_pos);
	m_window.setView(view);

	m_bgtilemap.setPosition({ player_pos.x - halfViewWidth, 0 });
}

void DebugLevel::render()
{
	beginDraw();
	m_bgtilemap.render(m_window);
	m_tilemap.render(m_window);
	m_window.draw(m_player);
	if (m_wormias.isAlive()) {
		m_window.draw(m_wormias);
	}
	
	endDraw();
}

void DebugLevel::onBegin() {

}
void DebugLevel::onEnd()
{
	std::cout << "Bye Debug Level\n";



	// reset player and level state
	m_player.reset();
	m_wormias.reset();
}
