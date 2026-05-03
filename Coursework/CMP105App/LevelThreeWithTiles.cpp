#include "LevelThreeWithTiles.h"


LevelThreeWithTiles::LevelThreeWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio)
	: LevelTemplate(window, input, gameState, audio), m_alertText(m_font), m_timer(m_font)
{

	setUpLevel(m_tilemap, m_bgtilemap, m_oneStarTime, m_twoStarTime, m_threeStarTime, "data/level3.txt", "data/level3TM.txt", "data/level3BG.txt", "gfx/tilemap.png", "gfx/tilemap-backgrounds.png");
	m_tilemap.buildLevel();
	m_bgtilemap.buildLevel();

	if (!m_tileTexture.loadFromFile("gfx/tilemap.png")) std::cerr << "no tile image found";

	//player set-up
	m_player.setAudio(&m_audio);
	m_player.setInput(&m_input);
	m_player.setEdges(0, WORLD_SIZE.x);
	m_player.setCanDoubleJump(true);
	m_player.setEndGamePosition({ 2 , 369 });	
	m_player.setLeverPosition({ 1860,9 });

	//goal flag set-up
	m_flag.setPosition({ 2,369 });
	m_flag.setTexture(&m_tileTexture);
	m_flag.setSize({ 72,72 });
	m_flag.setup();
	
	//lever set-up
	m_lever.setPosition({ 1860, 9 });
	m_lever.setTexture(&m_tileTexture);
	m_lever.setSize({ 72,72 });
	m_lever.setUsed(false);

	//m setup text
	if (!m_font.openFromFile("font/bitcount.ttf")) std::cerr << "no font found";
	m_alertText.setString("Seems there's a problem \nin that tree.");
	m_alertText.setPosition({ 50, 150 });
	m_alertText.setCharacterSize(36);
	m_alertText.setFillColor(sf::Color::Black);

	m_timer.setCharacterSize(20);
	m_timer.setFillColor(sf::Color::Black);

	//deploy the worms....
	for (int i = 0; i < 3; i++) {
		Worm* new_worm = new Worm();
		
		switch (i) {
		case 0: new_worm->setSpawnPos({ 654, 369}); break;
		case 1: new_worm->setSpawnPos({ 2746 , 297 }); break;
		case 2: new_worm->setSpawnPos({ 2148,9 }); new_worm->flip(); break;
		default: break;
		}
		m_wormList.push_back(new_worm);
	}
		
}

void LevelThreeWithTiles::handleInput(float dt)
{
	m_player.handleInput(dt);

	if (m_input.isPressed(sf::Keyboard::Scancode::Escape))
		m_gameState.setCurrentState(State::MENU);
	if (m_input.isPressed(sf::Keyboard::Scancode::Tab)) {
		if (m_timerActive) {
			m_timerActive = false;
		}
		else {
			m_timerActive = true;
		}
	}

	
}


void LevelThreeWithTiles::update(float dt) {
	m_player.update(dt);
	for (int i = 0; i < 3; i++) {
		m_wormList[i]->update(dt);
	}
	m_lever.update(dt);

	m_timer.setPosition({ m_player.getPosition().x , m_player.getPosition().y - (VIEW_SIZE.y / 8.f) + 10 });
	m_timeManager.addTime(dt);
	float roundedTime = round(m_timeManager.getCurrentTime() * 100) / 100; //SHOULD multiply time by 100 (1.234 -> 123.4) then round (123) then divide again by 100 (1.23) to round to 2 d.p
	std::string timerMessage = std::to_string(roundedTime);
	m_timer.setString(timerMessage);


	std::vector<GameObject>& level = *m_tilemap.getLevel();
	for (auto& t : level)
	{
		if (t.isCollider() && Collision::checkBoundingBox(m_player, t))
		{
			m_player.collisionResponse(t);
		}
		for (int i = 0; i < 3; i++) {
			if (t.isCollider() && Collision::checkBoundingBox(*m_wormList[i], t)) {
				m_wormList[i]->collisionResponse(t);
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		if (m_wormList[i]->isAlive()) {
			m_wormList[i]->playerCollision(m_player, m_audio);
		}
	}

	// show text if player has dropped very low down
	if (m_promptTimer > 0)
		m_promptTimer -= dt;
	else if (m_alertText.getString() != "")
	{
		// turn off prompt
		m_alertText.setString("");
	}
	else if (m_player.getPosition().y > WORLD_SIZE.y)
	{
		m_alertText.setCharacterSize(24);
		m_alertText.setPosition(m_window.getView().getCenter());
		m_alertText.setString("Press R to reset");
	}
	// show text if the player in lever range
	else if (m_player.inLeverRange())
	{
		m_alertText.setCharacterSize(24);
		m_alertText.setPosition(m_window.getView().getCenter() + sf::Vector2f(-100.f, -150.f));
		m_promptTimer = PROMPT_TIME;
		if (!m_flagLeverPulled)
			m_alertText.setString("Press F to fix\nthe tree");
		else
			m_alertText.setString("Better get\n back to \nthat flag!");
	}
	else if (m_player.inEndRange())
	{
		m_alertText.setCharacterSize(24);
		m_alertText.setPosition(m_window.getView().getCenter() + sf::Vector2f(-100.f, -150.f));
		m_promptTimer = PROMPT_TIME;
		if (m_flagLeverPulled)
			m_alertText.setString("Good job! Press\nF to end the day");
		else {
			m_alertText.setString("I need to \n fix the tree first!");
		}
	}

	if (m_player.getLeverPulled())
	{
		if (!m_flagLeverPulled) m_promptTimer = 0;
		m_flagLeverPulled = true;
		m_lever.setUsed(true);
	}
	else
	{
		m_lever.setUsed(false);
	}
	if (m_player.getGameEndTriggered())
	{
		m_timeManager.setFinalTime();
		m_gameState.setCurrentState(State::MENU);
	}


	// reset if fallen too far
	if (m_player.getPosition().y > 1200)
	{
		m_player.reset();
		m_audio.playSoundbyName("death");
	}


	updateCameraAndBackground();
}

void LevelThreeWithTiles::updateCameraAndBackground()
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

void LevelThreeWithTiles::render()
{
	beginDraw();
	m_bgtilemap.render(m_window);
	m_tilemap.render(m_window);
	m_window.draw(m_lever);
	m_window.draw(m_flag);
	m_window.draw(m_player);
	for (int i = 0; i < 3; i++) {
		if (m_wormList[i]->isAlive()) {
			m_window.draw(*m_wormList[i]);
		}
	}
	m_window.draw(m_alertText);
	if (m_timerActive) {
		m_window.draw(m_timer);
	}

	endDraw();
}

void LevelThreeWithTiles::onBegin() {
	//insert starting music here
}
void LevelThreeWithTiles::onEnd()
{



	// reset player and level state
	m_player.reset();
	for (int i = 0; i < 3; i++) {
		m_wormList[i]->reset();
	}
	m_flagLeverPulled = false;
	// reset alert text
	m_alertText.setString("Seems there's a problem \nin that tree.");
	m_alertText.setPosition({ 50, 150 });
	m_alertText.setCharacterSize(36);
	m_alertText.setFillColor(sf::Color::Black);
	m_promptTimer = PROMPT_TIME;
	// sfx
	m_audio.stopAllSounds();
	m_audio.stopAllMusic();
	m_timeManager.resetTime();
}
