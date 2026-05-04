#include "LevelWithTiles.h"

LevelWithTiles::LevelWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio)
	: LevelTemplate(window, input, gameState, audio), m_alertText(m_font), m_timer(m_font)
{

	setUpLevel(m_tilemap, m_bgtilemap, m_oneStarTime, m_twoStarTime, m_threeStarTime, "data/level1.txt", "data/level1TM.txt", "data/level1BG.txt", "gfx/tilemap.png", "gfx/tilemap-backgrounds.png");
	m_tilemap.buildLevel();
	m_bgtilemap.buildLevel(); 
	
	
	// setup text
	if (!m_font.openFromFile("font/bitcount.ttf")) std::cerr << "no font found";
	m_alertText.setString("Who keeps turning\nthe wind off?");
	m_alertText.setPosition({ 50, 150});
	m_alertText.setCharacterSize(36);
	m_alertText.setFillColor(sf::Color::Black);
	m_promptTimer = PROMPT_TIME;
	
	m_timer.setCharacterSize(20);
	m_timer.setFillColor(sf::Color::Black);

	

	if (!m_tileTexture.loadFromFile("gfx/tilemap.png")) std::cerr << "no tile image found";

	// setup flags and end game pos
	m_player.setEndGamePosition({ 24, 325 });
	for (int i = 0; i < 3; i++)
	{
		Flag* new_flag = new Flag();
		new_flag->setSize({ 72,72 });
		new_flag->setPosition({72.f + (i * 288), 100.f});
		new_flag->setTexture(&m_tileTexture);
		new_flag->setup();	// ensure first frame is good.
		m_flags.push_back(new_flag);
	}

	m_lever.setPosition({ 2730, 252 });
	m_lever.setTexture(&m_tileTexture);
	m_lever.setSize({ 72,72 });
	m_lever.setUsed(false);
	m_player.setLeverPosition({ 2730, 252 });
	m_player.setAudio(&m_audio);
	m_player.setInput(&m_input);
	m_player.setEdges(0, WORLD_SIZE.x);


	
}

void LevelWithTiles::handleInput(float dt)
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

void LevelWithTiles::update(float dt)
{
	m_timer.setPosition({ m_player.getPosition().x , m_player.getPosition().y - (VIEW_SIZE.y / 8.f) + 10 });
	m_timeManager.addTime(dt);
	float roundedTime = round(m_timeManager.getCurrentTime() * 100) / 100; //SHOULD multiply time by 100 (1.234 -> 123.4) then round (123) then divide again by 100 (1.23) to round to 2 d.p
	std::string timerMessage =  std::to_string(roundedTime);
	m_timer.setString(timerMessage);


	if (m_flagLeverPulled)
	{
		for (auto& flag : m_flags) flag->update(dt);
	}
	m_lever.update(dt);
	m_player.update(dt);


	std::vector<GameObject>& level = *m_tilemap.getLevel();
	for (auto& t : level)
	{
		if (t.isCollider() && Collision::checkBoundingBox(m_player, t))
		{
			m_player.collisionResponse(t);
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
			m_alertText.setString("Press F to fix\nthe wind");
		else
			m_alertText.setString("Better check\nthose flags");
	}
	else if (m_player.inEndRange())
	{
		m_alertText.setCharacterSize(24);
		m_alertText.setPosition(m_window.getView().getCenter() + sf::Vector2f(-100.f, -150.f));
		m_promptTimer = PROMPT_TIME;
		if (m_flagLeverPulled)
			m_alertText.setString("Good job! Press\nF to end the day");
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

	// camera follows player, bounded.
	updateCameraAndBackground();

}

void LevelWithTiles::updateCameraAndBackground()
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

void LevelWithTiles::render()
{
	beginDraw();
	m_bgtilemap.render(m_window);
	m_tilemap.render(m_window);
	m_window.draw(m_lever);
	for (auto& flag : m_flags) m_window.draw(*flag);
	m_window.draw(m_player);
	m_window.draw(m_alertText);
	if (m_timerActive) {
		m_window.draw(m_timer);
	}
	endDraw();
}

void LevelWithTiles::onBegin()
{
	m_audio.playMusicbyName("bgm1");
	
}

void LevelWithTiles::onEnd()
{

	//Check if level is left by beating 
	if (m_player.getGameEndTriggered()){

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
		while (saveFileRead >> type >> value){
			if (type == "CurrentLevel") {
				if (value < 2) {
					currentData = type + " 2\n";
				}
				else {
					currentData = type + std::to_string(value) + "\n";
				}
			}
			else if (type == "Level2Stars") {
				currentData += type + " " + std::to_string(value) + "\n";
			}
			else if (type == "Level3Stars") {
				currentData += type + " " + std::to_string(value);
			}
			else if (type == "Level1Stars") {
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
		
			saveFileWrite << currentData << "\nLevel1Stars " << starsAchieved;


			saveFileWrite.close();
		}


		saveFileRead.close();
		
	}

	// reset player and level state
	m_player.reset();
	m_flagLeverPulled = false;
	// reset alert text
	m_alertText.setString("Who keeps turning\nthe wind off?");
	m_alertText.setPosition({ 50, 150 });
	m_alertText.setCharacterSize(36);
	m_alertText.setFillColor(sf::Color::Black);
	m_promptTimer = PROMPT_TIME;
	// sfx
	m_audio.stopAllSounds();
	m_audio.stopAllMusic();
	m_timeManager.resetTime();
}
