
#include "BaseObject.h"

BaseObject::BaseObject()
{
	p_object_ = NULL;
	rect_.x = 0;
	rect_.y = 0;
	rect_.w = 0; // rộng
	rect_.h = 0; // cao
} // hàm gán giá trị ban đầu cho các biến

BaseObject::~BaseObject()
{
	Free();
} // hàm hủy

bool BaseObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	Free();
	SDL_Texture* new_texture = NULL; // biến kết quả
	// đọc tấm ảnh
	SDL_Surface* load_surface = IMG_Load(path.c_str());
	if (load_surface != NULL) // kiểm tra xem có lỗi không
	{
		SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
		// dùng color key để xóa nền tấm ảnh cho thật hơn
		// chuyển surface về texture 
		new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
		// lưu giữ toàn bộ thông tin tấm ảnh
		if (new_texture != NULL)
		{
			rect_.w = load_surface->w;
			rect_.h = load_surface->h;
		}
		SDL_FreeSurface(load_surface);

	}
	// gán
	p_object_ = new_texture;
	// p_object_ có cả thông tin tấm ảnh, kích thước,...

	return p_object_ != NULL;
	// kiểm tra p_object_ để return false hoặc true
}

void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip /* = NULL */)
{
	SDL_Rect renderquad = { rect_.x, rect_.y, rect_.w, rect_.h };
	// vị trí, kích thước tấm ảnh.
	SDL_RenderCopy(des, p_object_, clip, &renderquad);
	// đẩy toàn bộ thông số của p_object_ lên des với kích thước, vị trí trong renderquad
}
// biến des là để load tấm ảnh lên nền nào
void BaseObject::Free() // hàm giải phóng các đối tượng
{
	if (p_object_ != NULL)
	{
		SDL_DestroyTexture(p_object_);
		p_object_ = NULL;
		rect_.w = 0;
		rect_.h = 0;
	}
}
