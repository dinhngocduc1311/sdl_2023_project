
#include "CommonFunc.h"


int SDLCommonFunc::ShowMenu(SDL_Renderer* g_screen, TTF_Font* font,
    const std::string& menu1,
    const std::string& menu2,
    const std::string& img_name)
{
    char* ch1 = (char*)menu1.c_str();
    char* ch2 = (char*)menu2.c_str();
    char* img_file = (char*)img_name.c_str();

    int size1 = menu1.length();//size của dòng item menu
    int size2 = menu2.length();

    int time = 0;
    int x = 0;//tọa độ con chuột
    int y = 0;
    const int kMenuNum = 2;//biến số lượng item cho menu
    char* labels[kMenuNum];

    labels[0] = new char[size1 + 1];
    labels[1] = new char[size2 + 1];

    strcpy_s(labels[0], size1 + 1, ch1);
    strcpy_s(labels[1], size2 + 1, ch2);

    SDL_Texture* menu[kMenuNum];
    bool selected[kMenuNum] = { 0, 0 };//biến kiểm tra trạng thái khi focus: chọn hay không chọn
    SDL_Color color[2] = { { 255, 255, 255 },{ 255, 0, 0 } };//khai báo màu cơ bản

    //tạo text cho menu 
    TextObject text_object[kMenuNum];
    //khởi tạo ban đầu cho text : Play Game
    text_object[0].SetText(labels[0]);
    text_object[0].SetColor(color[0].r, color[0].g, color[0].b);//khởi tạo màu ban đầu
    text_object[0].LoadFromRenderText(font, g_screen);
    //khởi tạo ban đầu cho text : Exit
    text_object[1].SetText(labels[1]);
    text_object[1].SetColor(color[0].r, color[0].g, color[0].b);
    text_object[1].LoadFromRenderText(font, g_screen);


    SDL_Rect pos[kMenuNum];//vị trí xuất hiện của item menu
    pos[0].x = SCREEN_WIDTH * 0.3 - 350;//gán vị trí 
    pos[0].y = SCREEN_HEIGHT * 0.8 - 160;
    pos[1].x = SCREEN_WIDTH * 0.3 - 350;//gán vị trí
    pos[1].y = SCREEN_HEIGHT * 0.8 - 80;

    BaseObject gBackground;
    bool ret = gBackground.LoadImg(img_file, g_screen);

    SDL_Event event;//lấy event khi xử lí chuột
    while (true)
    {
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event))//vòng lặp sự kiện
        {
            switch (event.type)//các sự kiện 
            {
            case SDL_QUIT://tắt chương trình 
                text_object[0].Free();
                text_object[1].Free();
                gBackground.Free();
                return 1;//trả ra giá trị exit để hàm main check
            case SDL_MOUSEMOTION://con chuột di chuyển trên màn hình
                x = event.motion.x;//tọa độ con chuột
                y = event.motion.y;
                for (int i = 0; i < kMenuNum; ++i)//kiểm tra với từng item
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)//check xem có nằm trong vùng của item không
                    {
                        if (!selected[i])//nếu selected==false
                        {
                            selected[i] = 1;//được focus
                            text_object[i].SetText(labels[i]);//set lại text
                            text_object[i].SetColor(color[1].r, color[1].g, color[1].b);//đổi màu đi
                            text_object[i].LoadFromRenderText(font, g_screen);
                        }
                    }
                    else//không nằm trong vùng
                    {
                        if (selected[i])
                        {
                            selected[i] = 0;
                            text_object[i].Free();//giải phóng
                            text_object[i].SetText(labels[i]);
                            text_object[i].SetColor(color[0].r, color[0].g, color[0].b);
                            text_object[i].LoadFromRenderText(font, g_screen);
                        }
                    }
                }
                break;
                //khi bấm chuột vào item -> kết quả 
            case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;//tọa độ con chuột
                y = event.button.y;
                for (int i = 0; i < kMenuNum; ++i) // kiểm tra với từng item
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)//check xem có nằm trong vùng của item không
                    {
                        text_object[0].Free();
                        text_object[1].Free();
                        gBackground.Free();
                        return i;//trả về giá trị i khi bấm vào item i 
                    }
                }
                break;
                //chưa làm gì mà nhấn phím esc
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)//bấm esc 
                {
                    text_object[0].Free();
                    text_object[1].Free();
                    gBackground.Free();
                    return 1;//exit chương trình
                }
            }
        }

        gBackground.Render(g_screen, NULL);//load tấm ảnh menu
        //load menu item lên
        for (int i = 0; i < kMenuNum; ++i)
        {
            text_object[i].RenderText(g_screen, pos[i].x, pos[i].y);
            pos[i].w = text_object[i].GetWidth();
            pos[i].h = text_object[i].GetHeight();
        }


        SDL_RenderPresent(g_screen);
        if (1000 / 30 > (SDL_GetTicks() - time))
            SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
    }

    return 0;
}





int SDLCommonFunc::ShowMenuEnd(SDL_Renderer* g_screen, TTF_Font* font,
    const std::string& menu1,
    const std::string& img_name)
{
    char* ch1 = (char*)menu1.c_str();
    
    char* img_file = (char*)img_name.c_str();

    int size1 = menu1.length();
   

    int time = 0;
    int x = 0;
    int y = 0;
    const int kMenuNum = 1;
    char* labels[kMenuNum];

    labels[0] = new char[size1 + 1];
    

    strcpy_s(labels[0], size1 + 1, ch1);
    

    SDL_Texture* menu[kMenuNum];
    bool selected[kMenuNum] = {0};
    SDL_Color color[2] = { { 255, 255, 255 },{ 255, 0, 0 } };

    TextObject text_object[kMenuNum];
    text_object[0].SetText(labels[0]);
    text_object[0].SetColor(color[0].r, color[0].g, color[0].b);
    text_object[0].LoadFromRenderText(font, g_screen);

    

    SDL_Rect pos[kMenuNum];
    pos[0].x = SCREEN_WIDTH * 0.3 - 350;//gán vị trí
    pos[0].y = SCREEN_HEIGHT * 0.8 - 80;
    

    BaseObject gBackground;
    bool ret = gBackground.LoadImg(img_file, g_screen);

    SDL_Event event;
    while (1)
    {
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                text_object[0].Free();
                gBackground.Free();
                return 1;
            case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < kMenuNum; ++i)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        if (!selected[i])
                        {
                            selected[i] = 1;
                            text_object[i].SetText(labels[i]);
                            text_object[i].SetColor(color[1].r, color[1].g, color[1].b);
                            text_object[i].LoadFromRenderText(font, g_screen);
                        }
                    }
                    else
                    {
                        if (selected[i])
                        {
                            selected[i] = 0;
                            text_object[i].Free();
                            text_object[i].SetText(labels[i]);
                            text_object[i].SetColor(color[0].r, color[0].g, color[0].b);
                            text_object[i].LoadFromRenderText(font, g_screen);
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                for (int i = 0; i < kMenuNum; ++i) {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        text_object[0].Free();
                        
                        gBackground.Free();
                        return i;
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    text_object[0].Free();
                    
                    gBackground.Free();
                    return 1;
                }
            }
        }

        gBackground.Render(g_screen, NULL);

        for (int i = 0; i < kMenuNum; ++i)
        {
            text_object[i].RenderText(g_screen, pos[i].x, pos[i].y);
            pos[i].w = text_object[i].GetWidth();
            pos[i].h = text_object[i].GetHeight();
        }


        SDL_RenderPresent(g_screen);
        if (1000 / 30 > (SDL_GetTicks() - time))
            SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
    }

    return 0;
}

bool SDLCommonFunc::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)//hàm check va chạm
{
    //size object 1
    int left_a = object1.x;
    int right_a = object1.x + object1.w;
    int top_a = object1.y;
    int bottom_a = object1.y + object1.h;


    //size object 2
    int left_b = object2.x;
    int right_b = object2.x + object2.w;
    int top_b = object2.y;
    int bottom_b = object2.y + object2.h;

    // TH 1: size object 1 < size object 2: check tại 4 góc của hình ảnh 
    if (left_a > left_b && left_a < right_b)
    {
        if (top_a > top_b && top_a < bottom_b)
        {
            return true;
        }
    }

    if (left_a > left_b && left_a < right_b)
    {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {
            return true;
        }
    }

    if (right_a > left_b && right_a < right_b)
    {
        if (top_a > top_b && top_a < bottom_b)
        {
            return true;
        }
    }

    if (right_a > left_b && right_a < right_b)
    {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {
            return true;
        }
    }

    // TH 2: size object 1 < size object 2
    if (left_b > left_a && left_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {
            return true;
        }
    }

    if (left_b > left_a && left_b < right_a)
    {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
            return true;
        }
    }

    if (right_b > left_a && right_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {
            return true;
        }
    }

    if (right_b > left_a && right_b < right_a)
    {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
            return true;
        }
    }

    // TH 3: size object 1 = size object 2, 2 ảnh trùng nhau 
    if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
    {
        return true;
    }
    return false;
}