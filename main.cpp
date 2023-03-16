#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <string>


extern "C" {
#include"./sdl-2.0.7/include/SDL.h"
#include"./sdl-2.0.7/include/SDL_main.h"
}

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BOARD_SIZE 4

#define BOARD_WIDTH	300
#define BOARD_X 50
#define BOARD_Y 10

struct playerScore{
	int endScore;
	double endTime;
	int endSize;
};

void DrawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};

void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
};

void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
		};
	};


void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
	};

void DrawPoint(SDL_Surface *screen, int X, int Y, int width, const char *number, SDL_Surface *charset) {
	int length = strlen(number);
	DrawString(screen, (X + width / 2) - length*4, Y + width / 2, number, charset);
}

int** createtab(int size) {
	int **tab;
	tab = new int *[size];
	for (int i = 0; i < size; ++i) tab[i] = new int[size];

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			tab[i][j] = 0;
		}
	}
	int X = rand() % size;
	int Y = rand() % size;
	tab[X][Y] = 2;
	X = rand() % size;
	Y = rand() % size;
	tab[X][Y] = 2;

	return tab;
}

bool checkWin(int **&tab, int size) {
	bool win = 0;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (tab[i][j] == 2048) win = 1;
		}
	}

	return win;
}

bool checkLose(int **&tab, int size) {
	bool lose = 1;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - 1; j++) {
			if (tab[i][j] == tab[i][j + 1]) lose = 0;
		}
	}
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size; j++) {
			if (tab[i][j] == tab[i + 1][j]) lose = 0;
		}
	}

	if (checkPut(tab, size)) lose = 0;

	return lose;
}



int main(int argc, char **argv) {
	int size = 4;
	bool getStart = 0;
	unsigned int score = 0;
	unsigned int backScore = score;
	int showSize = 0;
	srand(time(NULL));
	int** tab;
	int** backtab;
	int fileSize = 0;
	playerScore *winnerList;

	int t1, t2, quit, frames, rc;
	double delta, worldTime, fpsTimer, fps, distance, etiSpeed;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *eti;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}


	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
	                                 &window, &renderer);
	if(rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
		};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "2048 by Tomasz Czochanski");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
	                              0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
	                           SDL_TEXTUREACCESS_STREAMING,
	                           SCREEN_WIDTH, SCREEN_HEIGHT);
}
