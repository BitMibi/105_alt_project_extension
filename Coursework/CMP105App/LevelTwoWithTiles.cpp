#include "LevelTwoWithTiles.h"

LevelTwoWithTiles::LevelTwoWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio)
	: LevelTemplate(window, input, gameState, audio), m_alertText(m_font), m_timer(m_font)
{
	setUpLevel(m_tilemap, m_bgtilemap, m_oneStarTime, m_twoStarTime, m_threeStarTime, "data/level2.txt", "data/level2TM.txt", "data/level2BG.txt", "gfx/tilemap.png", "gfx/tilemap-backgrounds.png");
	m_tilemap.buildLevel();
	m_bgtilemap.buildLevel();
	
	// setup player 
	m_player.setInput(&m_input);
	m_player.setEdges(0, WORLD_SIZE.x);
	m_player.setPosition({ 100, 100 });
	m_player.setAudio(&m_audio);


	// other bits
	sf::Vector2f boop_location = { 100, 72 * 2 + 100 };

	if (!m_tileTexture.loadFromFile("gfx/tilemap.png")) std::cerr << "failed to find tile images";
	m_coin.setTexture(&m_tileTexture);
	m_coin.setPosition(boop_location);
	m_coin.setSize({ 72,72 });
	m_coin.setAlive(false);
	m_coin.setCollisionBox({ { 0,0 }, { 72,72 } });

	m_boopBlock.setTexture(&m_tileTexture);
	m_boopBlock.setTextureRect({ {10 * 19,0}, { 18,18 } });
	m_boopBlock.setAlive(false);
	m_boopBlock.setCollisionBox({ { 0,0 }, { 72,72 } });
	m_boopBlock.setPosition(boop_location);
	m_boopBlock.setSize({ 72, 72 });

	m_flag.setPosition({ 38 * 72, 5 * 72 });
	m_flag.setSize({ 72,72 });
	m_flag.setTexture(&m_tileTexture);

	if (!m_font.openFromFile("font/bitcount.ttf")) std::cerr << "no font found";
	m_alertText.setPosition({ 50, 150 });
	m_alertText.setCharacterSize(36);
	m_alertText.setFillColor(sf::Color::Black);

	m_timer.setCharacterSize(20);
	m_timer.setFillColor(sf::Color::Black);
	

}

void LevelTwoWithTiles::onBegin()
{
	m_boopBlock.setAlive(false);
	m_coin.setAlive(false);
	m_player.setPosition({ 100, 100 });
	m_audio.playMusicbyName("bgm3");
	m_gameEnd = false;
}

void LevelTwoWithTiles::onEnd()
{
	// reset player
	m_player.setCanDoubleJump(false);
	// sfx
	m_audio.stopAllSounds();
	m_audio.stopAllMusic();

	//Check if level is left by beating 
	if (m_gameEnd) {
		m_player.setCanDoubleJump(true);	//Keep double jump unlocked
		std::string type;
		int value;
		std::string currentData; //String to append to the data to write to the file. yup. i think i know what im doing
		bool betterStars = true;

		m_timeManager.setFinalTime();
		int starsAchieved = m_timeManager.checkClearTime(m_oneStarTime, m_twoStarTime, m_threeStarTime);
		


		std::ifstream saveFileRead("data/save.txt");
		if (!saveFileRead.is_open()) {
			std::cerr << "uhhh. no save :broken_heart:";
		}
		while (saveFileRead >> type >> value) {
			if (type == "CurrentLevel") {
				if (value < 3) {
					currentData = type + " 3\n";
				}
				else {
					currentData = type + " " + std::to_string(value) + "\n";
				}
			}
			else if (type == "Level1Stars") {
				currentData += type + " " + std::to_string(value);
			}
			else if (type == "Level3Stars") {
				currentData += type + " " + std::to_string(value) + "\n";
			}
			else if (type == "Level2Stars") {
				if (starsAchieved <= value) {
					betterStars = false;
				}
			}
		}


		if (betterStars) {
			std::ofstream saveFileWrite("data/save.txt");
			if (!saveFileWrite.is_open()) {
				std::cerr << "houston. we've got a fucking disaster";
			}

			saveFileWrite << currentData << "\nLevel2Stars " << starsAchieved;


			saveFileWrite.close();
		}


		saveFileRead.close();

	}

	m_timeManager.resetTime();
	m_gameEnd = false;

}

void LevelTwoWithTiles::handleInput(float dt)
{
	m_player.handleInput(dt);

	//  I press escape. 
	if (m_input.isPressed(sf::Keyboard::Scancode::Escape))
	{
		
		// return to menu.
		m_gameState.setCurrentState(State::MENU);
	}
	//if I press F on the flag  
	if ((m_flag.getPosition() - m_player.getPosition()).length() < 75 &&
		m_input.isPressed(sf::Keyboard::Scancode::F)) {

		//set win level true
		m_gameEnd = true;
		//return to menu.
		m_gameState.setCurrentState(State::MENU);
	}
	if (m_input.isPressed(sf::Keyboard::Scancode::Tab)) {
		if (m_timerActive) {
			m_timerActive = false;
		}
		else {
			m_timerActive = true;
		}
	}
}

void LevelTwoWithTiles::update(float dt)
{
	m_timer.setPosition({ m_player.getPosition().x , m_player.getPosition().y - (VIEW_SIZE.y / 8.f) + 10 });
	m_timeManager.addTime(dt);
	float roundedTime = round(m_timeManager.getCurrentTime() * 100) / 100; //SHOULD multiply time by 100 (1.234 -> 123.4) then round (123) then divide again by 100 (1.23) to round to 2 d.p
	std::string timerMessage = std::to_string(roundedTime);
	m_timer.setString(timerMessage);

	m_player.update(dt);
	m_flag.update(dt);
	if (m_coin.isAlive()) m_coin.update(dt);

	// handle collisions
	std::vector<GameObject>& level = *m_tilemap.getLevel();
	for (auto& t : level)
	{
		if (t.isCollider() && Collision::checkBoundingBox(m_player, t))
		{
			m_player.collisionResponse(t);
		}	
	}

	if (m_boopBlock.isAlive())
	{
		if (Collision::checkBoundingBox(m_player, m_boopBlock))
		{
			if (m_player.getPosition().y >= m_boopBlock.getPosition().y)
			{
				// if booped from below
				m_boopBlock.setAlive(false);
				m_coin.setAlive(true);
			}
			m_player.collisionResponse(m_boopBlock);

		}
	}

	if (m_coin.isAlive())
	{
		if (Collision::checkBoundingBox(m_player, m_coin))
		{
			m_coin.setAlive(false);
			m_player.setCanDoubleJump(true);
		}
	}

	// turn block on when at wall.
	if ((m_wallPos - m_player.getPosition()).length() < 75)
	{
		m_boopBlock.setAlive(true);
	}

	// reset if fallen too far
	if (m_player.getPosition().y > 1200)
	{
		m_player.reset();
		m_audio.playSoundbyName("death");
	}

	updateCameraAndBackground();
	checkAndSetMessages();
}

void LevelTwoWithTiles::updateCameraAndBackground()
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

// sets prompt text and position 
void LevelTwoWithTiles::checkAndSetMessages()
{
	// get position 25% in from top and left (get middle, half both dimensions)
	sf::Vector2f inner_top_left = m_window.getView().getCenter();
	sf::Vector2f window_size = { 
		static_cast<float>(m_window.getSize().x), 
		static_cast<float>(m_window.getSize().y) };
	inner_top_left -= window_size * 0.25f;
	// big wall 
	if (!m_player.canDoubleJump() && 
		(m_wallPos - m_player.getPosition()).length() < 75)
	{
		m_alertText.setCharacterSize(24);
		m_alertText.setPosition(inner_top_left);
		m_alertText.setString(m_promptMessages[0]);
		
	}
	// double jump unlocked
	else if (m_player.canDoubleJump() && 
		(m_boopBlock.getPosition() - m_player.getPosition()).length() < 150)
	{
		m_alertText.setCharacterSize(24);
		m_alertText.setPosition(inner_top_left);
		m_alertText.setString(m_promptMessages[1]);
	}

	// complete level
	else if ((m_flag.getPosition() - m_player.getPosition()).length() < 75)
	{
		m_alertText.setCharacterSize(24);
		m_alertText.setPosition(inner_top_left);
		m_alertText.setString(m_promptMessages[2]);
	}
	else
	{
		m_alertText.setPosition({ 0,-100 });
	}
}

void LevelTwoWithTiles::render()
{
	beginDraw();
	m_bgtilemap.render(m_window);
	m_tilemap.render(m_window);
	if (m_boopBlock.isAlive()) m_window.draw(m_boopBlock);
	m_window.draw(m_flag);
	m_window.draw(m_player);
	if (m_coin.isAlive()) m_window.draw(m_coin);
	m_window.draw(m_alertText);
	if (m_timerActive) {
		m_window.draw(m_timer);
	}
	endDraw();
}