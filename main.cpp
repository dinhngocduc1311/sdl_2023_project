#define SDL_MAIN_HANDLED
#include"SDL.h"
#include"SDL_ttf.h"
#include<time.h>
#include<string>
#include<iostream>
#include<numeric>
#include <sstream>
static bool over = false;
namespace patch
{
	template <typename T> std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
}

SDL_Color white = { 255,255,255,255 };
class Tile {
public:
	int val;
	Tile() { if ((rand() % 11) > 8)val = 4; else val = 2; }
	Tile(int val) :val(val) {}
	const int GetVal() const { return val; }
};
class Board {
public:
	TTF_Font* font = TTF_OpenFont("Montserrat-Regular.ttf", 32);
	Tile* BoardMat[4][4];
	Board() {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				BoardMat[i][j] = NULL;
		spawn();
		spawn();
	}
	void draw(SDL_Renderer * renderer) {

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				SDL_Rect r = { 100 * i,100 * j,100,100 };
				if (BoardMat[i][j] != NULL)
				{
					int value = BoardMat[i][j]->GetVal();
					SDL_SetRenderDrawColor(renderer, (5 * value) % 256, (2 * value) % 256, (3 * value) % 256, 255);
					SDL_Surface* surface = NULL;
					SDL_Texture* texture = NULL;
					surface = TTF_RenderText_Blended(font, patch::to_string(value).c_str(), white);
					texture = SDL_CreateTextureFromSurface(renderer, surface);
					int recw, rech;
					SDL_QueryTexture(texture, NULL, NULL, &recw, &rech);
					SDL_Rect dstrect = { 100 * i + 29,100 * j + 29,recw,rech };
					SDL_RenderFillRect(renderer, &r);
					SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
					SDL_RenderCopy(renderer, texture, NULL, &dstrect);
					SDL_FreeSurface(surface);
					SDL_DestroyTexture(texture);
				}
				else SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
			}
	}
	void moveup()
	{

		for (int j = 0; j < 4; j++)
			for (int i = 0; i < 4; i++)
			{
				if (BoardMat[i][j] != NULL)
				{
					if (j > 0) {
						if (BoardMat[i][j - 1] != NULL) {
							if (BoardMat[i][j - 1]->val == BoardMat[i][j]->val) {
								BoardMat[i][j - 1]->val = 2 * (BoardMat[i][j - 1]->val);
								BoardMat[i][j] = NULL;
							}
						}
					}


					else continue;
				}

			}
		for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
				if (BoardMat[i][j] != NULL) {
					if (j > 0 && BoardMat[i][0] == NULL) {
						BoardMat[i][0] = BoardMat[i][j];
						BoardMat[i][j] = NULL;
					}
					else if (j > 1 && BoardMat[i][1] == NULL) {
						BoardMat[i][1] = BoardMat[i][j];
						BoardMat[i][j] = NULL;
					}
					else if (j > 2 && BoardMat[i][2] == NULL)
					{
						BoardMat[i][2] = BoardMat[i][j];
						BoardMat[i][j] = NULL;
					}
				}

	}
	void movedown()
	{
		for (int j = 3; j >= 0; j--)
			for (int i = 0; i < 4; i++)
			{
				if (BoardMat[i][j] != NULL)
				{
					if (j < 3) {
						if (BoardMat[i][j + 1] != NULL) {
							if (BoardMat[i][j + 1]->val == BoardMat[i][j]->val) {
								BoardMat[i][j + 1]->val = 2 * (BoardMat[i][j + 1]->val);
								BoardMat[i][j] = NULL;
							}
						}
					}
					else continue;
				}
			}
		for (int j = 3; j >= 0; j--)
			for (int i = 0; i < 4; i++)
				if (BoardMat[i][j] != NULL) {
					if (j < 4 && BoardMat[i][3] == NULL)
					{
						BoardMat[i][3] = BoardMat[i][j];
						BoardMat[i][j] = NULL;
					}
					else if (j < 3 && BoardMat[i][2] == NULL)
					{
						BoardMat[i][2] = BoardMat[i][j];
						BoardMat[i][j] = NULL;
					}
					else if (j < 2 && BoardMat[i][1] == NULL)
					{
						BoardMat[i][1] = BoardMat[i][j];
						BoardMat[i][j] = NULL;
					}
				}

	}
	void spawn() {
		int freesq = 0;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				if ((BoardMat[i][j] == NULL))freesq++;
			}
		if (!freesq) { over = gameover(); }
		while (freesq) {
			int row = rand() % 4;
			int col = rand() % 4;
			if (BoardMat[row][col] == NULL) { BoardMat[row][col] = new Tile(); break; }
		}
	}
	bool gameover() {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (BoardMat[i][j] != NULL) {
					if (j < 3 && BoardMat[i][j + 1]!=NULL)if (BoardMat[i][j]->val == BoardMat[i][j + 1]->val)return false;
					if (j > 0 && BoardMat[i][j - 1] != NULL)if (BoardMat[i][j]->val == BoardMat[i][j - 1]->val)return false;
					if (i < 3 && BoardMat[i+1][j] != NULL)if (BoardMat[i][j]->val == BoardMat[i + 1][j]->val)return false;
					if (i > 0 && BoardMat[i-1][j] != NULL)if (BoardMat[i][j]->val == BoardMat[i - 1][j]->val)return false;
				}
			}
		}
		return true;
	}
	void printend(SDL_Renderer*renderer) {
		SDL_Surface* surface = NULL;
		SDL_Texture* texture = NULL;
		surface = TTF_RenderText_Blended(font,"Game Over. Press Esc to quit.", white);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		int recw, rech;
		SDL_QueryTexture(texture, NULL, NULL, &recw, &rech);
		SDL_Rect dstrect = { 80,180,recw/2,rech/2 };
		SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
	}
	~Board() {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				delete BoardMat[i][j];
			}
		}

	}
};
int main(int* argc, char** argv)
{
	srand(unsigned int(time(NULL)));
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	SDL_Window* window = SDL_CreateWindow("2048", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 2);
	SDL_Event event;
	bool isRunning = true;
	Board board;
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	while (isRunning) {
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)isRunning = false;
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:isRunning = false; break;
                case SDLK_UP:board.moveup(); board.spawn(); break;
				case SDLK_DOWN:board.movedown(); board.spawn(); break;
				}
			}
			if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym) {
				case SDLK_RIGHT:break;
				}
			}
		}

		SDL_RenderClear(renderer);
		if (!over)board.draw(renderer);
		else board.printend(renderer);
		SDL_RenderPresent(renderer);
		/*char input = 'x';
		std::cin >> input;
		switch (input) {
        case 'w':board.moveup(); board.spawn(); break;
		case 's':board.movedown(); board.spawn(); break;
		}*/
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
