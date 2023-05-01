#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

// Lớp nền tảng để load ảnh lên màn hình, quản lý vấn đề hình ảnh
#include "CommonFunc.h"

class BaseObject
{
public:
	BaseObject(); // constructer
	~BaseObject(); // destructer
	void SetRect(const int& x, const int& y)
	{
		rect_.x = x;
		rect_.y = y;
	} // hàm set kích thước cho rect_
	SDL_Rect GetRect() const
	{
		return rect_;
	} // hàm lấy ra rect_ của tấm ảnh
	SDL_Texture* GetObject() const // dùng const để đảm bảo tính chặt chẽ, lấy chứ không có set 
	{
		return p_object_;
	} // hàm lấy ra p_object_
	virtual bool LoadImg(std::string path, SDL_Renderer* screen); 
		
	// load ảnh bigmap lên trên đối tượng baseobject sau đó thêm nó vào màn hình 
	// path là đường dẫn chứa ảnh, screen là biến màn hình
	// virtual là đề dùng luôn cho MainPlayerObject
	
	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);// đẩy toàn bộ thông số của p_object_ lên des với kích thước, vị trí trong renderquad
	void Free();//giải phóng

protected:
	SDL_Texture* p_object_; // biến lưu trữ các hình ảnh
	SDL_Rect rect_; // biến lưu kích thước tấm ảnh
};

#endif