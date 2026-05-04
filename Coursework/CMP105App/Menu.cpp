#include "Menu.h"

Menu::Menu(sf::RenderWindow& hwnd, Input& in, GameState& gs, AudioManager& aud) :
	Scene(hwnd, in, gs, aud), m_playButtonLabel(m_font), m_playButton2Label(m_font), m_resetButtonLabel(m_font), m_levelButtonLabel(m_font), m_exitButtonLabel(m_font), m_yesLabel(m_font), m_noLabel(m_font), m_playButton3Label(m_font), m_sureLabel(m_font)
{
	if (!m_font.openFromFile("font/bitcount.ttf"))
		std::cerr << "failed to load bitcount font";

	//Level Select Labels
	m_playButtonLabel.setCharacterSize(24);		// setup labels
	m_playButtonLabel.setPosition({ 185,53 });
	m_playButtonLabel.setString("Level 1");
	m_playButtonLabel.setFillColor(sf::Color::Black);
	m_playButton2Label.setCharacterSize(24);
	m_playButton2Label.setPosition({ 185,193 });
	m_playButton2Label.setString("Level 2");
	m_playButton2Label.setFillColor(sf::Color::Black);
	m_playButton3Label.setCharacterSize(24);
	m_playButton3Label.setPosition({ 185,333 });	
	m_playButton3Label.setString("Level 3");
	m_playButton3Label.setFillColor(sf::Color::Black);
	 
	//Main menu labels	
	m_resetButtonLabel.setCharacterSize(24);
	m_resetButtonLabel.setPosition({ 145,53 });
	m_resetButtonLabel.setString("New Game");
	m_resetButtonLabel.setFillColor(sf::Color::Black);
	m_levelButtonLabel.setCharacterSize(24);
	m_levelButtonLabel.setPosition({ 145,193 });
	m_levelButtonLabel.setString("Continue");
	m_levelButtonLabel.setFillColor(sf::Color::Black);
	m_exitButtonLabel.setCharacterSize(24);
	m_exitButtonLabel.setPosition({ 185,333 });
	m_exitButtonLabel.setString("Exit");
	m_exitButtonLabel.setFillColor(sf::Color::Black);

	//Exit Screen
	m_yesLabel.setCharacterSize(32);
	m_yesLabel.setPosition({ 128,303 });
	m_yesLabel.setString("YES");
	m_yesLabel.setFillColor(sf::Color::Black);
	m_noLabel.setCharacterSize(32);
	m_noLabel.setPosition({ 308,303 });
	m_noLabel.setString("NO");
	m_noLabel.setFillColor(sf::Color::Black);
	m_sureLabel.setCharacterSize(32);
	m_sureLabel.setPosition({ 85,93 });
	m_sureLabel.setString("Are you sure you \n want to quit?");
	m_sureLabel.setFillColor(sf::Color::White);

	//Level Select buttons
	m_playButton.setSize({ 216,100 });			// setup buttons
	m_playButton.setPosition({ 108,18 });
	m_playButton.setCollisionBox({ {0,0}, m_playButton.getSize()});
	m_playButton.setFillColor(m_defaultButtonColour); 
	m_play2Button.setSize({ 216,100 });			
	m_play2Button.setPosition({ 108,158 });
	m_play2Button.setCollisionBox({ {0,0}, m_playButton.getSize() });
	m_play2Button.setFillColor(m_defaultButtonColour);
	m_play3Button.setSize({ 216,100 });
	m_play3Button.setPosition({ 108,298 });
	m_play3Button.setCollisionBox({ {0,0}, m_playButton.getSize() });
	m_play3Button.setFillColor(m_defaultButtonColour);

	//Main Menu buttons
	m_resetButton.setSize({ 216,100 });
	m_resetButton.setPosition({ 108,18 });
	m_resetButton.setCollisionBox({ {0,0}, m_playButton.getSize() });
	m_levelButton.setSize({ 216,100 });
	m_levelButton.setPosition({ 108,158 });
	m_levelButton.setCollisionBox({ {0,0}, m_playButton.getSize() });
	m_levelButton.setFillColor(m_defaultButtonColour);
	m_resetButton.setFillColor(m_defaultButtonColour);
	m_exitButton.setSize({ 216,100  });
	m_exitButton.setPosition({ 108,298 });
	m_exitButton.setCollisionBox({ {0,0}, m_playButton.getSize() });
	m_exitButton.setFillColor(m_defaultButtonColour);
	

	//Exit Screen buttons 
	m_yesButton.setSize({ 110, 75 });
	m_yesButton.setPosition({ 108,298 });
	m_yesButton.setCollisionBox({ {0,0}, m_yesButton.getSize() });
	m_yesButton.setFillColor(m_defaultButtonColour);
	m_noButton.setSize({ 110, 75 });
	m_noButton.setPosition({ 278,298 });
	m_noButton.setCollisionBox({ {0,0}, m_yesButton.getSize() });
	m_noButton.setFillColor(m_defaultButtonColour);

	//setup stars
	for (int i = 0; i < 3; i++) {
		m_level1Stars[i] = new Star;
		m_level1Stars[i]->setSize({ 50,50 });
		m_level1Stars[i]->setPosition({ float((m_playButton.getPosition().x + m_playButton.getSize().x) - (50 * (2-i))), 78});	//Set the star positions
	}
	for (int i = 0; i < 3; i++) {
		m_level2Stars[i] = new Star;
		m_level2Stars[i]->setSize({ 50,50 });
		m_level2Stars[i]->setPosition({ float((m_play2Button.getPosition().x + m_play2Button.getSize().x) - (50 * (2-i))), 218 });	//Set the star positions
	}
	for (int i = 0; i < 3; i++) {
		m_level3Stars[i] = new Star;
		m_level3Stars[i]->setSize({ 50,50 });
		m_level3Stars[i]->setPosition({ float((m_play3Button.getPosition().x + m_play3Button.getSize().x) - (50 * (2 - i))), 358 });
	}

	if (!m_titleSplash.loadFromFile("gfx/title_splash.png")) std::cerr << "no splash found";
	m_titleImage.setTexture(&m_titleSplash);
	m_titleImage.setSize({ 432,432 });
}

void Menu::handleInput(float dt)
{
	sf::Vector2i mousePos{ m_input.getMouseX(), m_input.getMouseY()};
	if (m_currentMenu == MenuState::LEVELS) {
		if (m_input.isLeftMousePressed() &&
			Collision::checkBoundingBox(m_playButton, mousePos))
		{
			m_gameState.setCurrentState(State::LEVELONE);
		}
		if (m_input.isLeftMousePressed() &&
			Collision::checkBoundingBox(m_play2Button, mousePos) &&
			!m_locked2)
		{
			m_gameState.setCurrentState(State::LEVELTWO);
		}
		if (m_input.isLeftMousePressed() &&
			Collision::checkBoundingBox(m_play3Button, mousePos) &&
			!m_locked3) {
			m_gameState.setCurrentState(State::LEVELTHREE);
		}
		if (m_input.isPressed(sf::Keyboard::Scancode::Escape)) {
			m_currentMenu = MenuState::MAIN;
		}
	}
	else if (m_currentMenu == MenuState::MAIN) {
		if (m_input.isLeftMousePressed() &&
			Collision::checkBoundingBox(m_resetButton, mousePos)) {
			m_currentMenu = MenuState::LEVELS;
			newSave();
		}
		if (m_input.isLeftMousePressed() &&
			Collision::checkBoundingBox(m_levelButton, mousePos)) {
			m_currentMenu = MenuState::LEVELS;
		}
		if (m_input.isLeftMousePressed() &&
			Collision::checkBoundingBox(m_exitButton, mousePos)) {
			m_currentMenu = MenuState::EXIT;
		}
		if (m_input.isPressed(sf::Keyboard::Scancode::Escape)) {
			m_currentMenu = MenuState::EXIT;
		}
	}
	else if (m_currentMenu == MenuState::EXIT) {
		if (m_input.isLeftMousePressed() &&
			Collision::checkBoundingBox(m_yesButton, mousePos)) {
			m_window.close();
		}
		if (m_input.isLeftMousePressed() &&
			Collision::checkBoundingBox(m_noButton, mousePos)) {
			m_currentMenu = MenuState::MAIN;
		}
		if (m_input.isPressed(sf::Keyboard::Scancode::Escape)) {
			m_window.close();
		}
	}
	

	
}

void Menu::render()
{
	beginDraw();
	m_window.draw(m_titleImage);
	if (m_currentMenu == MenuState::LEVELS) {
		m_window.draw(m_playButton);
		for (int i = 0; i < 3; i++) {
			m_window.draw(*m_level1Stars[i]);
		}
		m_window.draw(m_playButtonLabel);
		m_window.draw(m_play2Button);
		for (int i = 0; i < 3; i++) {
			m_window.draw(*m_level2Stars[i]);
		}
		m_window.draw(m_playButton2Label);
		m_window.draw(m_play3Button);
		for (int i = 0; i < 3; i++) {
			m_window.draw(*m_level3Stars[i]);
		}
		m_window.draw(m_playButton3Label);
	}
	else if (m_currentMenu == MenuState::MAIN) {
		m_window.draw(m_resetButton);
		m_window.draw(m_resetButtonLabel);
		m_window.draw(m_levelButton);
		m_window.draw(m_levelButtonLabel);
		m_window.draw(m_exitButton);
		m_window.draw(m_exitButtonLabel);
	}
	else if (m_currentMenu == MenuState::EXIT) {
		m_window.draw(m_sureLabel);
		m_window.draw(m_yesButton);
		m_window.draw(m_yesLabel);
		m_window.draw(m_noButton);
		m_window.draw(m_noLabel);
	}
	

	endDraw();
}

void Menu::update(float dt)
{
	sf::Vector2i mousePos{ m_input.getMouseX(), m_input.getMouseY() };
	
	//LEVEL HOVER OVERS
	if (m_currentMenu == MenuState::LEVELS) {
		for (int i = 0; i < 3; i++) {
			m_level1Stars[i]->update(dt);
			m_level2Stars[i]->update(dt);
			m_level3Stars[i]->update(dt);
		}
		if (Collision::checkBoundingBox(m_playButton, mousePos))
		{
			m_playButton.setFillColor(m_hoverButtonColour);
		}
		else
		{
			m_playButton.setFillColor(m_defaultButtonColour);
		}
		if (!m_locked2) {
			if (Collision::checkBoundingBox(m_play2Button, mousePos))
			{
				m_play2Button.setFillColor(m_hoverButtonColour);
			}
			else
			{
				m_play2Button.setFillColor(m_defaultButtonColour);

			}
		}
		
		else {
			m_play2Button.setFillColor(m_lockedButtonColour);
		}
		if (!m_locked3) {
			if (Collision::checkBoundingBox(m_play3Button, mousePos))
			{
				m_play3Button.setFillColor(m_hoverButtonColour);
			}
			else
			{
				m_play3Button.setFillColor(m_defaultButtonColour);

			}
		}
		else {
			m_play3Button.setFillColor(m_lockedButtonColour);
		}
	}
	//MAIN HOVER OVERS
	else if (m_currentMenu == MenuState::MAIN) {
		
		if (Collision::checkBoundingBox(m_resetButton, mousePos)) {
			m_resetButton.setFillColor(m_hoverButtonColour);
		}
		else {
			m_resetButton.setFillColor(m_defaultButtonColour);
		}
		if (Collision::checkBoundingBox(m_levelButton, mousePos)) {
			m_levelButton.setFillColor(m_hoverButtonColour);
		}
		else {
			m_levelButton.setFillColor(m_defaultButtonColour);
		}
		if (Collision::checkBoundingBox(m_exitButton, mousePos)) {
			m_exitButton.setFillColor(m_hoverButtonColour);
		}
		else {
			m_exitButton.setFillColor(m_defaultButtonColour);
		}
	}
	//EXIT HOVER OVERS
	else if (m_currentMenu == MenuState::EXIT) {
		if (Collision::checkBoundingBox(m_yesButton, mousePos)) {
			m_yesButton.setFillColor(m_hoverButtonColour);
		}
		else {
			m_yesButton.setFillColor(m_defaultButtonColour);
		}
		if (Collision::checkBoundingBox(m_noButton, mousePos)) {
			m_noButton.setFillColor(m_hoverButtonColour);
		}
		else {
			m_noButton.setFillColor(m_defaultButtonColour);
		}
	}
	
}

void Menu::onBegin()
{
	auto view = m_window.getDefaultView();
	view.setCenter({ 216, 216 });
	m_window.setView(view);
	m_audio.playMusicbyName("bgm2");
	readSave();
}

void Menu::onEnd()
{
	m_audio.stopAllMusic();
}

void Menu::readSave() {
	std::string type;
	int value;

	std::ifstream saveFile("data/save.txt");
	if (!saveFile.is_open()) {
		std::cerr << "YOUR SAVE IS GONE!!! THERE'S NO SAVE!!!!!! AAAAAAAAAAAAA";
		newSave();
	}

	
	while (saveFile >> type >> value) {
		if (type == "CurrentLevel") {
			if (value == 1) {
				m_locked2 = true;
			}
			else if (value == 2) {
				m_locked2 = false;
			}
			else if (value == 3) {
				m_locked3 = false;
			}
		}
		else if (type == "Level1Stars")
			for (int i = 0; i < value; i++) {
				m_level1Stars[i]->setAchieved();
			}
		else if (type == "Level2Stars")
			for (int i = 0; i < value; i++) {
				m_level2Stars[i]->setAchieved();
			}
		else if (type == "Level3Stars")
			for (int i = 0; i < value; i++) {
				m_level3Stars[i]->setAchieved();
			}
	}

	saveFile.close();

}

void Menu::newSave() {
	std::ofstream newSaveFile("data/save.txt");
	if (!newSaveFile.is_open()) { std::cerr << "erm.. couldn't make new save.."; }
	newSaveFile << "CurrentLevel 1" << "\nLevel1Stars 0" << "\nLevel2Stars 0" << "\nLevel3Stars 0";
	newSaveFile.close();

	//Set values to new save file
	m_locked2 = true;
	m_locked3 = true;

	for (int i = 0; i < 3; i++) {
			m_level1Stars[i]->setUnachieved();
		}

	for (int i = 0; i < 3; i++) {
			m_level2Stars[i]->setUnachieved();
		}
	for (int i = 0; i < 3; i++) {
		m_level3Stars[i]->setUnachieved();
	}
}