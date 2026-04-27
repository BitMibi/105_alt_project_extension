#include "LevelTemplate.h"
#include <fstream>

 LevelTemplate::LevelTemplate(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio): Scene(window, input, gameState, audio) {

}

void LevelTemplate::updateCameraAndBackground(Player& playerRef, TileMap& bgRef)
{
	auto view = m_window.getView();
	auto player_pos = playerRef.getPosition() + playerRef.getSize() * 0.5f;

	float halfViewWidth = VIEW_SIZE.x / 2.0f;
	float halfViewHeight = VIEW_SIZE.y / 2.0f;

	player_pos.x = std::clamp(player_pos.x, halfViewWidth, WORLD_SIZE.x - halfViewWidth);
	player_pos.y = std::clamp(player_pos.y, halfViewHeight, WORLD_SIZE.y - halfViewHeight);

	view.setCenter(player_pos);
	m_window.setView(view);

	bgRef.setPosition({ player_pos.x - halfViewWidth, 0 });
}

void LevelTemplate::setUpLevel(TileMap& tmRef, TileMap& bgRef, std::string dataFileToLoad, std::string tileMapFile, std::string bgFile, std::string textureFile, std::string bgTexture) {

	GameObject tile;
	std::vector<GameObject> tileSet;

	
	//OPEN FILE

	//Used to read in data 
	std::string line;
	std::string type;
	int value1;
	int value2; //used for the map dimensions

	std::ifstream currentLevelData(dataFileToLoad);
	if (!currentLevelData.is_open()) {
		std::cerr << "no level dot txt file :(";
	}
	
	int num_columns = 1;	//Base values in case of an error 
	int num_rows = 1;
	int tile_size = 1;
	int sheet_spacing = 1;
	sf::Vector2u mapDimensions = { 1,1 };
	sf::Vector2f position = { 1,1 };

	//Read in the values in the txt 
	while (currentLevelData >> line >> type >> value1 >> value2) {
		if (line == "levelTiles") {
			if (type == "columns") { num_columns = value1; }
			else if (type == "rows") { num_rows = value1; }
			else if (type == "tile_size") { tile_size = value1; }      // Visual size of the tile
			else if (type == "sheet_spacing"){ sheet_spacing = value1; }  // Gap between tiles
			else if (type == "mapDimensions") { mapDimensions = { unsigned(value1), unsigned(value2) }; }
			else if (type == "position") {position = { float(value1), float(value2) }; break;}
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

	
	//Read in the tile map
	std::vector<int> tileMap;
	tileMap.clear();
	std::ifstream tileMapData(tileMapFile);
	if (!tileMapData.is_open()) {
		std::cerr << "no tile map dot txt file :(";
	}
	int tileValue;

	while (tileMapData >> tileValue) {
		
		if (tileValue >= 0) {
			tileMap.push_back(tileValue);

		}
		else if (tileValue == -1) {
			tileMap.push_back(b);

		}
	}
	if (tileMapData.is_open()) {tileMapData.close();}
	//Tile map file closed


	tmRef.loadTexture(textureFile);
	tmRef.setTileSet(tileSet);
	tmRef.setTileMap(tileMap, mapDimensions);
	tmRef.setPosition(position);
	//tmRef.buildLevel();

	tileSet.clear();
	tileMap.clear();


	//Read in the background values
	 while (currentLevelData >> line >> type >> value1 >> value2) {
		if (line == "background") {
			if (type == "columns") { num_columns = value1; }
			else if (type == "rows") { num_rows = value1; }
			else if (type == "tile_size") { tile_size = value1; }      // Visual size of the tile
			else if (type == "mapDimensions") { mapDimensions = { unsigned(value1), unsigned(value2) }; }
			else if (type == "position") { position = { float(value1), float(value2) }; }
		}

	}
	 	 
	
	 if (currentLevelData.is_open()) {
		 currentLevelData.close();
	}
	 //CLOSE DATA FILE
	
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

	//Read in the tile map
	
	std::ifstream bgTileData(bgFile);
	if (!bgTileData.is_open()) {
		std::cerr << "no tile map dot excel file :(";
	}

	while (bgTileData >> tileValue) {

		if (tileValue >= 0) {


			tileMap.push_back(tileValue);

		}
	}


	

	if (bgTileData.is_open()) { bgTileData.close(); }
	//Tile map file closed
	
	bgRef.loadTexture(bgTexture);
	bgRef.setTileSet(tileSet);
	bgRef.setTileMap(tileMap, mapDimensions);
	bgRef.setPosition(position);
	//bgRef.buildLevel();	

}
