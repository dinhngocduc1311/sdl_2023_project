
#include "GameMap.h"

void GameMap::LoadMap(char* name)
{
	FILE* fp = NULL;
	fopen_s(&fp, name, "rb"); // đọc file
	if (fp == NULL) return;
	game_map_.max_x_ = 0;
	game_map_.max_y_ = 0;
	for (int i = 0; i < MAX_MAP_Y; i++)
	{
		for (int j = 0; j < MAX_MAP_X; j++)
		{
			fscanf_s(fp, "%d", &game_map_.tile[i][j]); // đọc dữ liệu lưu vào trong game_map_.tile
			int val = game_map_.tile[i][j];
			if (val > 0) // không phải giá trị 0
			{
				if (j > game_map_.max_x_)
				{
					game_map_.max_x_ = j;
				}
				if (i > game_map_.max_y_)
				{
					game_map_.max_y_ = i;
				}
			}
		}
	}
	game_map_.max_x_ = (game_map_.max_x_ + 1) * TILE_SIZE;
	game_map_.max_y_ = (game_map_.max_y_ + 1) * TILE_SIZE;

	game_map_.start_x_ = 0; // vị trí hiện tại
	game_map_.start_y_ = 0;

	game_map_.file_name_ = name; // lưu lại
	fclose(fp); 
}

void GameMap::LoadTiles(SDL_Renderer* screen) // load img cho ảnh tương ứng
{
	char file_img[30];
	FILE* fp = NULL;
	for (int i = 0; i < MAX_TILES; i++)
	{
		sprintf_s(file_img, "map/%d.png", i); // chuyển đổi chỉ số i vào chuỗi 

		fopen_s(&fp, file_img, "rb");
		if (fp == NULL) // ảnh 0 đọc đc
		{
			continue;
		}
		fclose(fp); // khi ảnh đọc đc 
		tile_mat[i].LoadImg(file_img, screen); // load img lên
	}
}

void GameMap::DrawMap(SDL_Renderer* screen) // đưa hình ảnh vào ô
{
	int x1 = 0, x2 = 0;
	int y1 = 0, y2 = 0; // đưa từ vị trí nào đến vị trí nào

	int map_x = 0, map_y = 0;

	map_x = game_map_.start_x_ / TILE_SIZE; // vị trí ô thứ bao nhiêu 
	
	x1 = (game_map_.start_x_ % TILE_SIZE) * -1;
	x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

	map_y = game_map_.start_y_ / TILE_SIZE;

	y1 = (game_map_.start_y_ % TILE_SIZE) * -1;
	y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);

	for (int i = y1; i < y2; i += TILE_SIZE)
	{
		map_x = game_map_.start_x_ / TILE_SIZE;
		for (int j = x1; j < x2; j += TILE_SIZE)
		{
			int val = game_map_.tile[map_y][map_x];// trạng thái ô
			if (val > 0)
			{
				tile_mat[val].SetRect(j, i);// thiết lập vị trí
				tile_mat[val].Render(screen);// render lên màn hình
			}
			map_x++; // sau mỗi 1 vòng tăng chỉ số x,y
		}
		map_y++;
	}
}