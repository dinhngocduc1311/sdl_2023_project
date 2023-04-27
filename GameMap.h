
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define MAX_TILES 20
class TileMat : public BaseObject
{
public:
	TileMat() {;}
	~TileMat() {;}
};

class GameMap
{
public:
	GameMap() {;}
	~GameMap() {;}

	void LoadMap(char* name); // đọc thông tin file text map
	void LoadTiles(SDL_Renderer* screen); // load toàn bộ hình ảnh cho tile map
	void DrawMap(SDL_Renderer* screen); // xây dựng map
	Map getMap() const 
	{ 
		return game_map_; 
	};
	void SetMap(Map& map_data) { game_map_ = map_data; };

private:
	Map game_map_; // chứa thông tin về map: trạng thái ô, vị trí, chỉ số 
	TileMat tile_mat[MAX_TILES]; // mảng đối tượng lưu trữ hình ảnh
};
#endif


