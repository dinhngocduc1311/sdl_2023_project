#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

// chứa thông số chung cho project
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "BaseObject.h"
#include "TextObject.h"
// biến global
static SDL_Window* g_window = NULL; // biến xây dựng form window cho game
static SDL_Renderer* g_screen = NULL;//biến màn hình 
static SDL_Event g_event;//biến sự kiện

//biến quản lí âm thanh 
static Mix_Chunk* g_sound_bullet[2];
static Mix_Chunk* g_sound_exp[2];
static Mix_Chunk* g_sound[2];

// Screen : Màn hình game
const int FRAME_PER_SECOND = 30; // fps
const int SCREEN_WIDTH = 1280; // chiều rộng màn hình game
const int SCREEN_HEIGHT = 640; // chiều cao
const int SCREEN_BPP = 32; // thông số pixel / điểm ảnh

// mã màu
const int COLOR_KEY_R = 167; 
const int COLOR_KEY_G = 175; 
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0Xff; // màu sắc cho render ra màn hình



#define BLANK_TILE 0
#define TILE_SIZE 64 // kích thước mắt lưới

#define MAX_MAP_X 400 // chiều X của map là 400 
#define MAX_MAP_Y 10  // chiều Y của map là 10

#define STATE_MONEY 4
#define BOSS_EGG 3
#define RED_DIAMOND 10



typedef struct Input // cấu trúc dữ liệu lưu trạng thái di chuyển
{
	int left_, right_, up_, down_;
	int jump_;
};

typedef struct Map // thể hiện trạng thái, vị trí, chỉ số ô tile map
{
	int start_x_, start_y_; // vị trí start x, y

	int max_x_, max_y_; // chỉ số ô max_x=start_x_/tile_size

	int tile[MAX_MAP_Y][MAX_MAP_X]; // mảng 2 chiều lưu trạng thái các ô
	char* file_name_; // tên file 

};


namespace SDLCommonFunc
{
	bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);//hàm chuyên xử lí va chạm
	int ShowMenu(SDL_Renderer* g_screen, TTF_Font* font,
		const std::string& menu1,
		const std::string& menu2,
		const std::string& img_name);//hàm show menu gồm ảnh và text ra màn hình
	int ShowMenuEnd(SDL_Renderer* g_screen, TTF_Font* font,
		const std::string& menu1,
		const std::string& img_name);
}
#endif