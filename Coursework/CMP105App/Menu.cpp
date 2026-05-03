#include "Menu.h"

Menu::Menu(sf::RenderWindow& hwnd, Input& in, GameState& gs, AudioManager& aud) :
	Scene(hwnd, in, gs, aud), m_playButtonLabel(m_font), m_playButton2Label(m_font), m_resetButtonLabel(m_font)
{
	if (!m_font.openFromFile("font/bitcount.ttf"))
		std::cerr << "failed to load bitcount font";

	m_playButtonLabel.setCharacterSize(24);		// setup labels
	m_playButtonLabel.setPosition({ 185,93 });
	m_playButtonLabel.setString("Level 1");
	m_playButtonLabel.setFillColor(sf::Color::Black);
	m_playButton2Label.setCharacterSize(24);
	m_playButton2Label.setPosition({ 185,233 });
	m_playButton2Label.setString("Level 2");
	m_playButton2Label.setFillColor(sf::Color::Black);
	m_resetButtonLabel.setCharacterSize(14);
	m_resetButtonLabel.setPosition({ 340,375 });
	m_resetButtonLabel.setString("Reset Save");
	m_resetButtonLabel.setFillColor(sf::Color::Black);


	m_playButton.setSize({ 216,100 });			// setup buttons
	m_playButton.setPosition({ 108,58 });
	m_playButton.setCollisionBox({ {0,0}, m_playButton.getSize()});
	m_playButton.setFillColor(m_defaultButtonColour); 
	m_play2Button.setSize({ 216,100 });			
	m_play2Button.setPosition({ 108,198 });
	m_play2Button.setCollisionBox({ {0,0}, m_playButton.getSize() });
	m_play2Button.setFillColor(m_defaultButtonColour);
	m_resetButton.setSize({ 110, 75 });
	m_resetButton.setPosition({ 324,350 });
	m_resetButton.setCollisionBox({ {0,0}, m_playButton.getSize() });
	m_resetButton.setFillColor(m_defaultButtonColour);


	//setup stars
	for (int i = 0; i < 3; i++) {
		m_level1Stars[i] = new Star;
		m_level1Stars[i]->setSize({ 50,50 });
		m_level1Stars[i]->setPosition({ float((m_playButton.getPosition().x + m_playButton.getSize().x) - (50 * (2-i))), 118});	//Set the star positions
	}
	for (int i = 0; i < 3; i++) {
		m_level2Stars[i] = new Star;
		m_level2Stars[i]->setSize({ 50,50 });
		m_level2Stars[i]->setPosition({ float((m_play2Button.getPosition().x + m_play2Button.getSize().x) - (50 * (2-i))), 258 });	//Set the star positions
	}

	if (!m_titleSplash.loadFromFile("gfx/title_splash.png")) std::cerr << "no splash found";
	m_titleImage.setTexture(&m_titleSplash);
	m_titleImage.setSize({ 432,432 });
}

void Menu::handleInput(float dt)
{
	sf::Vector2i mousePos{ m_input.getMouseX(), m_input.getMouseY()};
	if(m_input.isLeftMousePressed() && 
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
	if(m_input.isLeftMousePressed() && 
		Collision::checkBoundingBox(m_resetButton, mousePos)) {
		newSave();
	}
	if (m_input.isPressed(sf::Keyboard::Scancode::Z)) {
		m_gameState.setCurrentState(State::LEVELTHREE);
	}
	if (m_input.isPressed(sf::Keyboard::Scancode::X)) {
		m_gameState.setCurrentState(State::DEBUG);
	}
	if (m_input.isPressed(sf::Keyboard::Scancode::Escape)) {
		m_window.close();
	}
}

void Menu::render()
{
	beginDraw();
	m_window.draw(m_titleImage);
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
	m_window.draw(m_resetButton);
	m_window.draw(m_resetButtonLabel);

	endDraw();
}

void Menu::update(float dt)
{
	sf::Vector2i mousePos{ m_input.getMouseX(), m_input.getMouseY() };
	for (int i = 0; i < 3; i++) {
		m_level1Stars[i]->update(dt);
		m_level2Stars[i]->update(dt);
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
	if (Collision::checkBoundingBox(m_resetButton, mousePos)) {
		m_resetButton.setFillColor(m_hoverButtonColour);
	}
	else {
		m_resetButton.setFillColor(m_defaultButtonColour);
	}
}

void Menu::onBegin()
{
	std::cout << "starting menu\n";
	auto view = m_window.getDefaultView();
	view.setCenter({ 216, 216 });
	m_window.setView(view);
	m_audio.playMusicbyName("bgm2");
	readSave();
}

void Menu::onEnd()
{
	//std::cout << "leaving menu\n";
	m_audio.stopAllMusic();
}

void Menu::readSave() {
	std::string type;
	int value;

	std::ifstream saveFile("data/save.txt");
	if (!saveFile.is_open()) {
		std::cerr << "FUCK YOUR SAVE IS GONE!!! THERE'S NO SAVE!!!!!! AAAAAAAAAAAAA";
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
		}
		if (type == "Level1Stars")
			for (int i = 0; i < value; i++) {
				m_level1Stars[i]->setAchieved();
			}
		if (type == "Level2Stars")
			for (int i = 0; i < value; i++) {
				m_level2Stars[i]->setAchieved();
			}
	}

	saveFile.close();

}

void Menu::newSave() {
	std::ofstream newSaveFile("data/save.txt");
	if (!newSaveFile.is_open()) { std::cerr << "erm.. couldn't make new save.."; }
	newSaveFile << "CurrentLevel 1" << "\nLevel1Stars 0" << "\nLevel2Stars 0";
	newSaveFile.close();

	//Set values to new save file
	m_locked2 = true;

	for (int i = 0; i < 3; i++) {
			m_level1Stars[i]->setUnachieved();
		}

	for (int i = 0; i < 3; i++) {
			m_level2Stars[i]->setUnachieved();
		}
}