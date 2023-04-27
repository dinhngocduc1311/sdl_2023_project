#ifndef PLAYER_POWER_H_
#define PLAYER_POWER_H_
//Hiển thị các hình ảnh tĩnh như money,egg ở text và sinh mạng cho nvat chính
#include "CommonFunc.h"
#include "BaseObject.h"


class PlayerPower : public BaseObject
{
public:
	PlayerPower();
	~PlayerPower();

	void SetNum(const int& num)
	{
		number_ = num;
	}
	void AddPos(const int& xPos);
	void Show(SDL_Renderer* screen);
	void Init(SDL_Renderer* screen);
	
	void Decrease();//hàm giảm mạng xuống

private:
	int number_;//chỉ số bao nhiêu mạng
	std::vector<int> pos_list_;//vị trí đặt ô hình ảnh mạng 

};


class PlayerMoney : public BaseObject
{
public:
	PlayerMoney();
	~PlayerMoney();

	void Init(SDL_Renderer* screen);//vị trí ban đầu
	void Show(SDL_Renderer* screen);//show ảnh
	void SetPos(const int& x, const int& y)
	{
		x_pos_ = x;
		y_pos_ = y;
	}
private:
	
	int x_pos_;//vị trí đặt ảnh tiền
	int y_pos_;
};

class PlayerEgg :public BaseObject
{
public:
	PlayerEgg();
	~PlayerEgg();

	void Init(SDL_Renderer* screen);
	void Show(SDL_Renderer* screen);
	void SetPos(const int& x, const int& y)
	{
		x_pos_ = x;
		y_pos_ = y;
	}
private:
	
	int x_pos_;
	int y_pos_;
};


class PlayerDiamond :public BaseObject
{
public:
	PlayerDiamond();
	~PlayerDiamond();

	void Init(SDL_Renderer* screen);
	void Show(SDL_Renderer* screen);
	void SetPos(const int& x, const int& y)
	{
		x_pos_ = x;
		y_pos_ = y;
	}
private:

	int x_pos_;
	int y_pos_;
};
#endif
