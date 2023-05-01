

#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_
//Đưa các text ra màn hình game 
#include "CommonFunc.h"

class TextObject
{
public:
	TextObject();
	~TextObject();


	enum TextColor //các màu cho text
	{
		RED_TEXT = 0,
		WHITE_TEXT = 1,
		BLACK_TEXT = 2,
	};

	
	bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);//load text lên màn hình
	void Free();//giải phóng

	void SetColor(Uint8 red, Uint8 green, Uint8 blue);//set mã màu khác 3 màu đã có
	void SetColor(int type);//set màu cơ bản
	
	void RenderText(SDL_Renderer* screen, 
		            int xp, int yp, 
		            SDL_Rect* clip = NULL, 
		            double angle = 0.0, 
		            SDL_Point* center = NULL, 
		            SDL_RendererFlip flip = SDL_FLIP_NONE);//show text lên màn hình
	int GetWidth() const
	{
		return width_;
	}
	int GetHeight() const
	{
		return height_;
	}

	void SetText(const std::string& text)
	{
		str_val_ = text;
	}//set nội dung text
	std::string GetText() const
	{
		return str_val_;
	}


private:
	std::string str_val_;//lưu nội dung text 
	SDL_Color text_color_;//set màu cho text
	SDL_Texture* texture_;//quản lý text
	int width_;//chiều rộng text
	int height_;//chiều cao text

};
#endif
