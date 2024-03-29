﻿
#include "MainPlayerObject.h"

MainPlayerObject::MainPlayerObject()
{
	frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = WALK_NONE;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.jump_ = 0;
	input_type_.down_ = 0;
	input_type_.up_ = 0;
	on_ground_ = false;
	map_x_ = 0;
	map_y_ = 0;
	come_back_time_ = 0;
	money_count = 0;
	egg_count = 0;
}// giá trị cơ bản 

MainPlayerObject::~MainPlayerObject()
{

}

bool MainPlayerObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);

	if (ret == true)
	{
		width_frame_ = rect_.w / PLAYER_NUM_FRAME; // lấy của 1 frame nên /8
		height_frame_ = rect_.h;

	}
	return ret;
}

SDL_Rect MainPlayerObject::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;

	return rect;
}

void MainPlayerObject::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0)
	{
		for (int i = 0; i < PLAYER_NUM_FRAME; ++i)
		{
			frame_clip_[i].x = width_frame_ * i;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}

	}
}

void MainPlayerObject::Show(SDL_Renderer* des)
{
	UpdateImagePlayer(des);
	
	if (input_type_.left_ == 1 || input_type_.right_ == 1) // khi nhân vật chuyển động liên tục
	{
		frame_++;
	}
	else frame_ = 0;
	if (frame_ >= 8) frame_ = 0;
	if (come_back_time_ == 0)// đỡ mất tgian xử lí khi nhân vật không xuất hiện trên bản đồ 
	{
		rect_.x = x_pos_ - map_x_;// hiển thị nhân vật đứng đúng vị trị trên màn hình 
		rect_.y = y_pos_ - map_y_;

		SDL_Rect* current_clip = &frame_clip_[frame_];//đang ở clip nào, ở frame thứ mấy 

		SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ }; // chính xác trạng thái đang ở tọa độ nào, rộng cao bao nhiêu

		SDL_RenderCopy(des, p_object_, current_clip, &renderQuad); //đẩy p_object lên màn hình với current_clip liên tục thay đổi từ 0 đén 7 
	}
	
}

void MainPlayerObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* bullet_sound[2])
{
	if (events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			status_ = WALK_RIGHT;
			input_type_.right_ = 1;
			input_type_.left_ = 0;
			UpdateImagePlayer(screen);
		}
		break;
		case SDLK_LEFT:
		{
			status_ = WALK_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			UpdateImagePlayer(screen);
		}
		break;
		}
	}
	else if (events.type == SDL_KEYUP)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			input_type_.right_ = 0;

		}
		break;
		case SDLK_LEFT:
		{
			input_type_.left_ = 0;
		}
		break;
		}
	}


	if (events.type == SDL_MOUSEBUTTONDOWN)
	{
		if (events.button.button == SDL_BUTTON_RIGHT)// nhảy từ chuột phải
		{
			input_type_.jump_ = 1;
		}
		else if (events.button.button == SDL_BUTTON_LEFT)//đạn bắn ra từ chuột trái
		{
			BulletObject* p_bullet = new BulletObject();//tạo viên đạn mới
			if (red_dia_count % 2 != 0)
			{
				p_bullet->set_bullet_type(BulletObject::RED_BULLET);//loại đạn 
				p_bullet->LoadImgBullet(screen);//load ảnh cho đạn
				Mix_PlayChannel(-1, bullet_sound[0], 0);// âm thanh khi bắn đạn
			}
			else
			{
				p_bullet->set_bullet_type(BulletObject::SPHERE_BULLET);//loại đạn 
				p_bullet->LoadImgBullet(screen);//load ảnh cho đạn
				Mix_PlayChannel(-1, bullet_sound[0], 0);// âm thanh khi bắn đạn
			}

			if (status_ == WALK_LEFT)//nhân vật di chuyển sang trái 
			{
				p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);//đạn bắn từ phải qua trái
				p_bullet->SetRect(this->rect_.x, rect_.y + height_frame_ * 0.25);//set vị trí viên đạn xuất phát từ đâu/ từ nòng súng
			}
			else//nhân vật di chuyển sang phải
			{
				p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
				p_bullet->SetRect(this->rect_.x + width_frame_ - 20, rect_.y + height_frame_ * 0.25);
			}
			
			
			p_bullet->set_x_val(20);//set tốc độ đạn 
			p_bullet->set_y_val(20);
			p_bullet->set_is_move(true);//gán trạng thái đạn, cho phép bắn

			p_bullet_list_.push_back(p_bullet);// nạp đạn vào băng 
		}
	}
}


void MainPlayerObject::HandleBullet(SDL_Renderer* des)// xử lí đạn bắn ra
{
	for (int i = 0; i < p_bullet_list_.size(); i++)//kiểm tra xem băng đạn có hay không
	{
		BulletObject* p_bullet = p_bullet_list_.at(i);//lấy ra viên đạn
		if (p_bullet != NULL)
		{
			if (p_bullet->get_is_move() == true)//cho phép bắn
			{
				p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);//xử lí đạn trong màn hình
				p_bullet->Render(des);//show đạn lên màn hình
			}
			else//không có 
			{
				p_bullet_list_.erase(p_bullet_list_.begin() + i);// hủy đạn đi
				if (p_bullet != NULL)
				{
					delete p_bullet;
					p_bullet = NULL;
				}
				                                 

			}
		}
	}
}

void MainPlayerObject::RemoveBullet(const int& idx)
{
	int size = p_bullet_list_.size();//kiểm tra băng đạn có đạn không
	if (size > 0 && idx < size)
	{
		BulletObject* p_bullet = p_bullet_list_.at(idx);//gọi viên đạn
		p_bullet_list_.erase(p_bullet_list_.begin() + idx);//xóa đạn khỏi danh sách

		if (p_bullet)//hủy 
		{
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}
void MainPlayerObject::DoPlayer(Map& map_data, Mix_Chunk* sound_player[2])
{
	//đang không rơi xuống vực
	if (come_back_time_ == 0)
	{
		x_val_ = 0;
		y_val_ += GRAVITY_SPEED; // nhân vật rơi từ từ

		if (y_val_ >= MAX_FALL_SPEED)
		{
			y_val_ = MAX_FALL_SPEED;

		}

		if (input_type_.left_ == 1)
		{
			x_val_ -= PLAYER_SPEED;
		}
		else if (input_type_.right_ == 1)
		{
			x_val_ += PLAYER_SPEED;
		}
		//nhảy 
		if (input_type_.jump_ == 1)
		{
			if (on_ground_ == true)
			{
				y_val_ = -PLAYER_JUMP_VAL;
				Mix_PlayChannel(-1, sound_player[1], 0);
			}
			on_ground_ = false;
			input_type_.jump_ = 0;
		}

		CheckToMap(map_data);
		CenterEntityOnMap(map_data);
	}
	//nvat ngoài vùng bản đồ
	if (come_back_time_ > 0)
	{
		come_back_time_--;
		if (come_back_time_ == 0) //Reset trang thai
		{
			on_ground_ = false;
			if (x_pos_ > 4 * TILE_SIZE)
			{
				x_pos_ -= 4 * TILE_SIZE; // trừ 4 o tile map
			}
			else
			{
				x_pos_ = 0;//Xuất phát từ ban đầu 
			}
			y_pos_ = 0;//Xuất hiện trở lại, chạy lại từ đầu
			x_val_ = 0;
			y_val_ = 0;
			Mix_PlayChannel(-1, sound_player[1], 0);
		}
	}
}

void MainPlayerObject::CenterEntityOnMap(Map& map_data)
{
	map_data.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);//vị trí đầu tiên của map, di chuyển đến nửa bản đồ thì bản đồ cuốn theo
	if (map_data.start_x_ < 0)
	{
		map_data.start_x_ = 0;
	}
	else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_) 
	{
		map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH; //Không cho bản đồ di chuyển thêm
	}

	map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);
	if (map_data.start_y_ < 0)
	{
		map_data.start_y_ = 0;
	}
	else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
	{
		map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
	}
}

void MainPlayerObject::CheckToMap(Map& map_data)
{
	int x1 = 0;
	int x2 = 0; // giới hạn kiểm tra từ x1 đến x2
	int y1 = 0;
	int y2 = 0;
	
	
	
	//Check chiều ngang 
	int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;
    
	x1 = (x_pos_ + x_val_) / TILE_SIZE; // ô hiện tại
	x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;// trừ đi 1 sai số để check va chạm tốt hơn 

	y1 = (y_pos_) / TILE_SIZE;
	y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

	
	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{// x1,y1 có nằm trong bản đồ không
		if (x_val_ > 0) // di chuyển về bên phải
		{
			int val1 = map_data.tile[y1][x2];
			int val2 = map_data.tile[y2][x2];

			if (val1 == STATE_MONEY || val2 == STATE_MONEY)//check va chạm của nvat chính và vàng
			{
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;//xóa vàng đi thay bằng ô trống
				IncreaseMoney();//cộng dồn 1 vàng
				
			}
			else if (val1 == BOSS_EGG || val2 == BOSS_EGG)//check va chạm của nvat chính và trứng rồng
			{
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;//xóa trứng đi thay bằng ô trống
				IncreaseEgg();//cộng dồn 1 trứng
				
			}
			else if (val1 == RED_DIAMOND || val2 == RED_DIAMOND)//check va chạm của nvat chính và kcuong đỏ
			{
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;//xóa đi thay bằng ô trống
				IncreaseRedDiamond();//cộng dồn 1
			
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					x_pos_ = x2 * TILE_SIZE; // dừng tại vị trí biên nhân vật 
					x_pos_ -= width_frame_ + 1;  // trở về vị trí có thể đứng được
					x_val_ = 0;
					
				}
			}
		}
		else if (x_val_ < 0) // lùi 
		{
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y2][x1];

			if (val1 == STATE_MONEY || val2 == STATE_MONEY)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y2][x1] = 0;
				IncreaseMoney();
				
			}
			else if (val1 == BOSS_EGG || val2 == BOSS_EGG)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y2][x1] = 0;
				IncreaseEgg();
				
			}
			else if (val1 == RED_DIAMOND || val2 == RED_DIAMOND)//check va chạm của nvat chính và kcuong đỏ
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y2][x1] = 0;//xóa đi thay bằng ô trống
				IncreaseRedDiamond();//cộng dồn 1
				
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					x_pos_ = (x1 + 1) * TILE_SIZE;
					x_val_ = 0;
					
				}
			}
		}

	}

	//Check chiều dọc
	int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
	x1 = (x_pos_) / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;

	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;

	

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
		//x1,y1 có trong bản đồ không
	{
		if (y_val_ > 0)
		{

			int val1 = map_data.tile[y2][x1];
			int val2 = map_data.tile[y2][x2];

			if (val1 == STATE_MONEY || val2 == STATE_MONEY)
			{
				map_data.tile[y2][x1] = 0;
				map_data.tile[y2][x2] = 0;
				IncreaseMoney();
				
			}
			else if (val1 == BOSS_EGG || val2 == BOSS_EGG)
			{
				map_data.tile[y2][x1] = 0;
				map_data.tile[y2][x2] = 0;
				IncreaseEgg();
			}
			else if (val1 == RED_DIAMOND || val2 == RED_DIAMOND)//check va chạm của nvat chính và kcuong đỏ
			{
				map_data.tile[y2][x1] = 0;
				map_data.tile[y2][x2] = 0;//xóa đi thay bằng ô trống
				IncreaseRedDiamond();//cộng dồn 1
				
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					y_pos_ = y2 * TILE_SIZE;
					y_pos_ -= (height_frame_ + 1);
					y_val_ = 0;
					on_ground_ = true;
					if (status_ == WALK_NONE)
					{
						status_ = WALK_RIGHT;
					}
					
				}
			}
		}
		else if (y_val_ < 0) // nhảy lên trên
		{
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y1][x2];
			if (val1 == STATE_MONEY || val2 == STATE_MONEY)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y1][x2] = 0;
				IncreaseMoney();
			
			}
			else if (val1 == BOSS_EGG || val2 == BOSS_EGG)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y1][x2] = 0;
				IncreaseEgg();
				
			}
			else if (val1 == RED_DIAMOND || val2 == RED_DIAMOND)//check va chạm của nvat chính và kcuong đỏ
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y1][x2] = 0;//xóa đi thay bằng ô trống
				IncreaseRedDiamond();//cộng dồn 1
			
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					y_pos_ = (y1 + 1) * TILE_SIZE;
					y_val_ = 0;
					
				}
			}
		}
	}

	
	//Nếu không va chạm
	x_pos_ += x_val_;
	y_pos_ += y_val_;

	if (x_pos_ < 0) //Lùi quá giới hạn
	{
		x_pos_ = 0;
	}
	else if (x_pos_ + width_frame_ > map_data.max_x_) // Tiến quá giới hạn
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;

	}
	//kiểm tra rơi xuống vực
	if (y_pos_ > map_data.max_y_)
	{
		come_back_time_ = 60;

	}
}

void MainPlayerObject::IncreaseMoney()
{
	money_count++;
	
}
void MainPlayerObject::IncreaseEgg()
{
	egg_count++;
}
void MainPlayerObject::IncreaseRedDiamond()
{
	red_dia_count++;
}


void MainPlayerObject::UpdateImagePlayer(SDL_Renderer* des) // kiểm tra trạng thái status
{
	if (on_ground_ == true)//trên mặt đất
	{
		if (status_ == WALK_LEFT)
		{
			LoadImg("img//player_left.png", des);
		}
		else LoadImg("img//player_right.png", des);

	}
	else//đang trên không trung
	{
		if (status_ == WALK_LEFT)
		{
			LoadImg("img//jum_left.png", des);

		}
		else LoadImg("img//jum_right.png", des);
	}
}