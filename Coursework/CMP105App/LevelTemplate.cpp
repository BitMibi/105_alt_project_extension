#include "LevelTemplate.h"
#include <fstream>

 LevelTemplate::LevelTemplate(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio): Scene(window, input, gameState, audio), m_alertText(m_font) {

}

void LevelTemplate::updateCameraAndBackground()
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

void LevelTemplate::setUpLevel(std::string fileToLoad, std::string textureFile, std::string bgTexture) {

	GameObject tile;
	std::vector<GameObject> tileSet;
	
	//OPEN FILE

	//Used to read in data 
	std::string line;
	std::string type;
	int value;

	std::ifstream currentLevel(fileToLoad);
	if (!currentLevel.is_open()) {
		std::cerr << "no level dot txt file :(";
	}
	
	int num_columns;
	int num_rows;
	int tile_size;
	int sheet_spacing;
	sf::Vector2u mapDimensions;
	//Read in the ints. somehow. i uh forgor
	while (currentLevel >> line >> type >> value) {
		if (line == "levelTiles") {
			if (type == "colmuns") { num_columns = value; }
			if (type == "rows") { num_rows = value; }
			if (type == "tile_size") { tile_size = value; }      // Visual size of the tile
			if (type == "sheet_spacing"){ sheet_spacing = value; }  // Gap between tiles
		}
	}


	// Set GameObject size (Scaling up 4x for visibility)
	// 4 * 18 = 3 * 24 = 72 (dino size is 24).
	tile.setSize(sf::Vector2f(tile_size * 4, tile_size * 4));
	tile.setCollisionBox({ { 0,0 }, tile.getSize() });

	
	

	for (int i = 0; i < num_columns * num_rows; i++)
	{
		int row = i / num_columns;
		int col = i % num_columns;

		tile.setTextureRect({
			{(tile_size + sheet_spacing) * col, (tile_size + sheet_spacing) * row},
			{tile_size, tile_size} });
		if (col <= 4 || col >= 12) tile.setCollider(true);
		else tile.setCollider(false);
		tileSet.push_back(tile);
	}

	// Add Blank
	tile.setTextureRect({ {0, 0}, {-24, -24} }); // Empty rect for blank
	int b = tileSet.size();
	tile.setCollider(false);
	tileSet.push_back(tile);

	int value2; //used for the map dimensions
	while (currentLevel >> line >> type >> value >> value2) {
		if (line == "levelTiles") {
			if (type == "mapDimensions") {
				mapDimensions = { value,value2 };
			}
		}
	}
	std::vector<int> tileMap;
	
	while (currentLevel >> type) {
		if (type == "tilemap") {
			while(std::getline(currentLevel, line)){}
			
		}
	}

	/*TO BE DELETED
	sf::Vector2u mapDimensions{ 40, 8 };
	std::vector<int> tileMap = {
		b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b, b, b, b  , b  , b  , b  , b  , b  , b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  ,
		b  , 131, b  , b  , b  , 131, b  , b  , b  , 131,  21,  22,  23, b, b, b,  21,  22,  22,  22,  22,  23, b, b, b,  21,  22,  22,  23, b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  ,
		b  , 131, b  , b  , b  , 131, b  , 21 ,  22, 22 , 121, 122, 123, b, b, b, 121, 122, 122, 122, 122, 123, b, b, b, 121, 122, 122, 123, 22 , 22 , 23 , b  , b  , b  , b  , b  , b  , b  , b  ,
		b  , 131, b  , b  , 21 ,  22, 22 , 121, 122, 122, 121, 122, 123, b, b, b, 121, 122, 122, 122, 122, 123, b, b, b, 121, 122, 122, 123, 122, 122, 123, 22 , 22 , 22 , 22 , 22 , 22 , 22 , 23 ,
		21 ,  22, 22 , 22 , 121, 122, 122, 121, 122, 122, 121, 122, 123, b, b, b, 121, 122, 122, 122, 122, 123, b, b, b, 121, 122, 122, 123, 122, 122, 123, 122, 122, 122, 122, 122, 122, 122, 123,
		121, 122, 122, 122, 121, 122, 122, 121, 122, 122, 121, 122, 123, b, b, b, 121, 122, 122, 122, 122, 123, b, b, b, 121, 122, 122, 123, 122, 122, 123, 122, 122, 122, 122, 122, 122, 122, 123,
		121, 122, 122, 122, 121, 122, 122, 121, 122, 122, 121, 122, 123, b, b, b, 121, 122, 122, 122, 122, 123, b, b, b, 121, 122, 122, 123, 122, 122, 123, 122, 122, 122, 122, 122, 122, 122, 123,
		141, 142, 142, 142, 141, 142, 142, 141, 142, 142, 141, 142, 143, b, b, b, 141, 142, 142, 142, 142, 143, b, b, b, 141, 142, 142, 143, 142, 142, 143, 142, 142, 142, 142, 142, 142, 142, 143
	}; */


	m_tilemap.loadTexture(textureFile);
	m_tilemap.setTileSet(tileSet);
	m_tilemap.setTileMap(tileMap, mapDimensions);
	m_tilemap.setPosition({ 0, 100 });
	m_tilemap.buildLevel();

	tileSet.clear();

	// setup background
	tile_size = 24;
	num_columns = 8;
	num_rows = 3;
	// 24 * 9 = 216, a multiple of 72, the LCM of the player and tile size.
	tile.setSize(sf::Vector2f(tile_size * 9, tile_size * 9));

	for (int i = 0; i < num_columns * num_rows; i++)
	{
		int row = i / num_columns;
		int col = i % num_columns;

		tile.setTextureRect({
			{(tile_size + sheet_spacing) * col, (tile_size + sheet_spacing) * row},
			{tile_size, tile_size} });
		tile.setCollider(false);		// don't collide with background
		tileSet.push_back(tile);
	}

	mapDimensions = { 14,3 };
	tileMap = {
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		14,14,14,14,14,14,14,14,14,14,14,14,14,14,
		22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22
	};
	m_bgtilemap.loadTexture(bgTexture);
	m_bgtilemap.setTileSet(tileSet);
	m_bgtilemap.setTileMap(tileMap, mapDimensions);
	m_bgtilemap.setPosition({ 0, 0 });
	m_bgtilemap.buildLevel();

	// setup player 
	m_player.setInput(&m_input);
	m_player.setEdges(0, WORLD_SIZE.x);
	m_player.setAudio(&m_audio);




	currentLevel.close();
}

void LevelTemplate::tileMapReading(std::string& line, std::vector<int>& tileMap,int b) {
	int tileValue;
	if (tileValue >= 0) {


		tileMap.push_back(tileValue);

	}
	else if (tileValue == -1) {
		tileMap.push_back(b);

	}
}