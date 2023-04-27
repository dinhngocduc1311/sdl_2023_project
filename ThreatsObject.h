#ifndef THREATS_OBJECT_H_
#define THREATS_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"
#define THREAT_GRAVITY_SPEED 0.8
#define THREAT_FRAME_NUM 8
#define THREAT_MAX_FALL_SPEED 10
#define THREAT_SPEED 3
class ThreatsObject : public BaseObject
{
public:
	ThreatsObject();
	~ThreatsObject();

	enum TypeMove //Kiểu di chuyển threats
	{
		STATIC_THREAT = 0,
		MOVE_IN_SPACE_THREAT = 1,

	};

	void set_x_val(const float& xVal)
	{
		x_val_ = xVal;
	}//set rect giống main player object
	void set_y_val(const float& yVal)
	{
		y_val_ = yVal;
	}
	void set_x_pos(const float& xp)
	{
		x_pos_ = xp;
	}
	void set_y_pos(const float& yp)
	{
		y_pos_ = yp;
	}
	float get_x_pos() const
	{
		return x_pos_;
	}
	float get_y_pos() const
	{
		return y_pos_;
	}
	void SetMapXY(const int& mp_x, const int& mp_y)
	{
		map_x_ = mp_x;
		map_y_ = mp_y;
	}//set map 

	void set_clips();
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	int get_width_frame() const
	{
		return width_frame_;
	}
	int get_height_frame() const
	{
		return height_frame_;
	}
	void DoPlayer(Map& gMap);//xử lí quái di chuyển 
	void CheckToMap(Map& gMap);
	void InitThreats();
	void set_type_move(const int& typeMove)
	{
		type_move_ = typeMove;
	}//set rect kiểu di chuyển 
	void SetAnimationPos(const int& pos_a, const int& pos_b)
	{
		animation_a_ = pos_a;
		animation_b_ = pos_b;
	}//set vị trí để di chuyển 
	void set_input_left(const int& ipLeft)
	{
		input_type_.left_ = ipLeft;
	}
	void ImpMoveType(SDL_Renderer* screen);//Hàm xử lí vấn đề di chuyển trong phạm vi a b
	SDL_Rect GetRectFrame();

	std::vector<BulletObject*> get_bullet_list() const
	{
		return bullet_list_;

	}//quản lý việc truyền vào và lấy đạn ra
	void set_bullet_list(const std::vector<BulletObject*>& bl_list)
	{
		bullet_list_ = bl_list;
	}//sử dụng cho xử lí va chạm
	void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen);//thông số của đạn
	void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit);
	void RemoveBullet(const int& idx);
	
	
private:

	int map_x_;
	int map_y_;//giới hạn của quái với bản đồ
	float x_val_;
	float y_val_;//biến tăng giá trị x,y
	float x_pos_;
	float y_pos_;//vị trí đang là bao nhiêu
	bool on_ground_;
	int come_back_time_;
	SDL_Rect frame_clip_[THREAT_FRAME_NUM];//
	int width_frame_;//frame kích thước bao nhiêu
	int height_frame_;
	int frame_;//quản lý chỉ số frame

	int type_move_;//kiểu di chuyển của threat
	int animation_a_;//giới hạn a trái, b phải
	int animation_b_;
	Input input_type_;//biến quản lý việc di chuyển 
	
	std::vector<BulletObject*> bullet_list_;//biến quản lí đạn bắn cho threats
};

#endif
