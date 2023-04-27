
#include "PlayerPower.h"

PlayerPower::PlayerPower()
{
	number_ = 0;
}
PlayerPower::~PlayerPower()
{

}

void PlayerPower::AddPos(const int& xp)
{
	pos_list_.push_back(xp);
}

void PlayerPower::Init(SDL_Renderer* screen)
{
	LoadImg("img//player_pw.png", screen);
	number_ = 3;//số mạng
	if (pos_list_.size() > 0)
	{
		pos_list_.clear();
	}
	AddPos(20);//add mặc định vị trí ban đầu cho mạng
	AddPos(60);
	AddPos(100);
}//hàm load tấm ảnh

void PlayerPower::Show(SDL_Renderer* screen)
{
	for (int i = 0; i < pos_list_.size(); i++)
	{
		rect_.x = pos_list_.at(i);
		rect_.y = 0;
		Render(screen);
	}
}//show tấm ảnh mạng theo vị trí pos_list

void PlayerPower::Decrease()
{
	number_--;
	pos_list_.pop_back();//loại bỏ vị trí cuối
}//hàm giảm mạng



PlayerMoney::PlayerMoney()
{
	x_pos_ = 0;
	y_pos_ = 0;
}//giá trị khởi tạo

PlayerMoney::~PlayerMoney()
{

}

void PlayerMoney::Init(SDL_Renderer* screen)
{
	bool ret = LoadImg("img/money.png", screen);

}//load ảnh

void PlayerMoney::Show(SDL_Renderer* screen)
{
	rect_.x = x_pos_;
	rect_.y = y_pos_;
	Render(screen);
}//show vị trí tấm ảnh

PlayerEgg::PlayerEgg()
{
	x_pos_ = 0;
	y_pos_ = 0;
}

PlayerEgg::~PlayerEgg()
{

}

void PlayerEgg::Init(SDL_Renderer* screen)
{
	bool ret = LoadImg("img/egg.png", screen);

}

void PlayerEgg::Show(SDL_Renderer* screen)
{
	rect_.x = x_pos_;
	rect_.y = y_pos_;
	Render(screen);
}


PlayerDiamond::PlayerDiamond()
{
	x_pos_ = 0;
	y_pos_ = 0;
}

PlayerDiamond::~PlayerDiamond()
{

}

void PlayerDiamond::Init(SDL_Renderer* screen)
{
	bool ret = LoadImg("img/diamond.png", screen);

}

void PlayerDiamond::Show(SDL_Renderer* screen)
{
	rect_.x = x_pos_;
	rect_.y = y_pos_;
	Render(screen);
}