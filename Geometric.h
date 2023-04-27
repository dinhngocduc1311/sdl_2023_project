
#ifndef GEOMETRIC_H_
#define GEOMETRIC_H_
//tạo 1 vùng riêng biệt ngoài rìa của game: tạo 1 hcn bao phủ hết các text time, mark, egg 
#include "CommonFunc.h"

typedef struct GeometricFormat//kiểu dữ liệu 
{
public:
	GeometricFormat(int left, int top, int width, int height)
	{
		left_ = left;
		top_ = top;
		width_ = width;
		height_ = height;
	}//constructer gán giá trị 

	int left_;//hcn gồm rộng,dài,cao
	int top_;
	int width_;
	int height_;

};

typedef struct ColorData//hình có màu sắc ra sao
{
public:
	ColorData(Uint8 r, Uint8 g, Uint8 b)
	{
		red_ = r;
		green_ = g;
		blue_ = b;
	}

	Uint8 red_;
	Uint8 green_;
	Uint8 blue_;
};

//class đối tượng hình học nói chung
class Geometric//dnghia hàm chuyên dùng để vẽ
{
public:
	static void RenderRectangle(const GeometricFormat& geo_size, const ColorData& color_data, SDL_Renderer* screen);//hcn
	static void RenderOutline(const GeometricFormat& geo_size, const ColorData& color_data, SDL_Renderer* screen);//hàm thể hiện đường viền cho hcn
};

#endif