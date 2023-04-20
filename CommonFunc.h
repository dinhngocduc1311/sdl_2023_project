#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_


#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "BaseObject.h"
#include "TextObject.h"

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;


static Mix_Chunk* g_sound_bullet[2];
static Mix_Chunk* g_sound_exp[2];
static Mix_Chunk* g_sound[2];

// Screen
const int FRAME_PER_SECOND = 45; // fps
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0Xff;



#define BLANK_TILE 0
#define TILE_SIZE 64

#define MAX_MAP_X 400
#define MAX_MAP_Y 10

#define STATE_MONEY 4
#define BOSS_EGG 3



struct Input
{
	int left_, right_, up_, down_;
	int jump_;
};

typedef struct Map
{
	int start_x_, start_y_;

	int max_x_, max_y_;

	int tile[MAX_MAP_Y][MAX_MAP_X];
	char* file_name_;

};


namespace SDLCommonFunc
{
	bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
	int ShowMenu(SDL_Renderer* g_screen, TTF_Font* font,
		const std::string& menu1,
		const std::string& menu2,
		const std::string& img_name);
	int ShowMenuEnd(SDL_Renderer* g_screen, TTF_Font* font,
		const std::string& menu1,
		const std::string& img_name);
}
#endif