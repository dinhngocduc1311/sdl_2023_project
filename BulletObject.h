
#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_
//hàm xử lí đạn bắn cho các nhân vật 
#include "BaseObject.h"
#include "CommonFunc.h"

class BulletObject : public BaseObject
{
public:
	BulletObject();
	~BulletObject();


	enum BulletDir //hướng của đạn 
	{
		DIR_RIGHT = 20,
		DIR_LEFT = 21,

	};
	enum BulletType //kiểu của đạn
	{
		SPHERE_BULLET = 50,
		LASER_BULLET = 51,
		RED_BULLET = 52,
	};
	void set_x_val(const int& xVal)//hàm set rect cho đạn 
	{
		x_val_ = xVal;
	}
	void set_y_val(const int& yVal)
	{
		y_val_ = yVal;
	}
	int get_x_val() const
	{
		return x_val_;
	}
	int get_y_val() const
	{
		return y_val_;
	}

	void set_is_move(const bool& isMove)
	{
		is_move_ = isMove;
	}// set rect cho is_move
	bool get_is_move() const
	{
		return is_move_;
	}

	void set_bullet_dir(const unsigned int& bulletDir)
	{
		bullet_dir_ = bulletDir;
	}//set rect cho bullet_dir
	unsigned int get_bullet_dir() const
	{
		return bullet_dir_;
	}
	void set_bullet_type(const unsigned int& bulletType)
	{
		bullet_type_ = bulletType;
	}//set rect cho bullet_type
	unsigned int get_bullet_type() const
	{
		return bullet_type_;
	}
	

	void HandleMove(const int& x_border, const int& y_border);//xử lí di chuyển cho viên đạn
	//khi đạn rời quá vùng hiển thị thì biến mất đi với 2 biến giới hạn 
	bool LoadImgBullet(SDL_Renderer* des);//load ảnh loại đạn bắn ra
private:
	int x_val_;//biến lưu giá trị thay đổi khi di chuyển 
	int y_val_;
	bool is_move_;//biến trạng thái có move không để xóa đạn khi rời khỏi màn hình
	unsigned int bullet_dir_;//hướng của đạn
	unsigned int bullet_type_;//kiểu của đạn
};




#endif
