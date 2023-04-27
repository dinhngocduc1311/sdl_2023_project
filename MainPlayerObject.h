
#ifndef MAIN_PLAYER_OBJECT_H_
#define MAIN_PLAYER_OBJECT_H_
// nhân vật game chính
#include <vector>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"


#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 8
#define PLAYER_JUMP_VAL 18
#define PLAYER_NUM_FRAME 8


class MainPlayerObject : public BaseObject
{
public:
	MainPlayerObject();
	~MainPlayerObject();

	enum WalkType //kiểu cho trạng thái di chuyển 
	{
		WALK_NONE = 0,
		WALK_RIGHT = 1,
	    WALK_LEFT = 2,
	};
	bool LoadImg(std::string path, SDL_Renderer* screen); // thông số riêng kích thước tấm ảnh 
	void Show(SDL_Renderer* des); // show theo từng frame, tăng dần frame nhân vật 
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* bullet_sound[2]); // hàm xử lí sự kiện khi bấm nút 
	void set_clips(); // xử lý animation, tạo hiệu ứng clip
	void DoPlayer(Map& map_data, Mix_Chunk* sound_player[2]); // cộng trừ 1 lượng cho nhân vật di chuyển
	void CheckToMap(Map& map_data); // kiểm tra xem nhân vật với map va chạm nhau
	void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; }
	//
	void CenterEntityOnMap(Map& map_data);// thông số của map khi di chuyển nhân vật
	void UpdateImagePlayer(SDL_Renderer* des);// hàm xử lí ảnh dựa trên sự kiện 
	SDL_Rect GetRectFrame();//hàm lấy rect frame cho nvat game

	void set_bullet_list(std::vector<BulletObject*> bullet_list)
	{
		p_bullet_list_ = bullet_list;

	}// set rect cho băng đạn 
	std::vector<BulletObject*> get_bullet_list() const
	{
		return p_bullet_list_;
	}
	void HandleBullet(SDL_Renderer* des);
	void RemoveBullet(const int& idx);//remove đạn thứ bao nhiêu
	void IncreaseMoney();
	void IncreaseEgg();
	void IncreaseRedDiamond();
	int get_frame_width() const
	{
		return width_frame_;
	}
	int get_frame_height() const
	{
		return height_frame_;
	}
	void set_comeback_time(const int& cb_time)
	{
		come_back_time_ = cb_time;
	}
	int GetMoneyCount() const
	{
		return money_count;
	}
	int GetEggCount() const
	{
		return egg_count;
	}
	int GetDiaCount() const
	{
		return red_dia_count;
	}
private:
	int red_dia_count;
	int money_count;
	int egg_count;
	std::vector<BulletObject*> p_bullet_list_;//biến băng đạn 
	float x_val_, y_val_; // ấn phải trái sẽ lùi tăng lên bao nhiêu
	float x_pos_, y_pos_; // vị trí hiện tại của nhân vật
	int width_frame_; // biến lưu kích thước 1 frame
	int height_frame_; 
	SDL_Rect frame_clip_[8]; // lưu các frame
	Input input_type_; // biến lưu trạng thái di chuyển của nhân vật
	int frame_; // lưu chỉ số frame
	int status_; // lưu trạng thái status 2 kiểu di chuyển phải/trái
	bool on_ground_; // kiểm tra trạng thái trên mặt đất

	int map_x_; // lưu vị trí bản đồ
	int map_y_;

	int come_back_time_;//biến kiểm tra quá trình rơi, tgian quay lại 
};

#endif