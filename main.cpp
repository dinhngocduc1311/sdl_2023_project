
#include "CommonFunc.h"
#include "BaseObject.h"
#include "GameMap.h"
#include "MainPlayerObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "PlayerPower.h"
#include "Geometric.h"
#include <Windows.h>
#include <WinUser.h>



BaseObject g_background; // biến lưu tấm ảnh và load lên màn hình
TTF_Font* font_time = NULL;//biến lưu font chữ hiện lên màn hình khi chơi
TTF_Font* font_MENU = NULL;//biến lưu font chữ hiện lên menu



bool InitData() // khởi động thông số môi trường sdl
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO); // thiết lập môi trường ban đầu sdl
    if (ret < 0) return false; // hàm lỗi
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // thiết lập chế độ tỉ lệ, chất lượng
    // tạo cửa số window
    g_window = SDL_CreateWindow("22028166", // tên
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SCREEN_WIDTH, // rộng
                                 SCREEN_HEIGHT, // cao
                                 SDL_WINDOW_SHOWN); 
    
    if (g_window == NULL) success = false;
    
    else 
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED); // tạo render cho biến g_screen
        if (g_screen == NULL) success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags)) success = false; // kiểm tra 
        }

        if (TTF_Init() == -1)
        {
            success = false;
        }//khởi tạo điều kiện của text

        //tạo đối tượng font chữ
        font_time = TTF_OpenFont("font//dlxfont.ttf", 15);//đọc file font 
        if (font_time == NULL)
        {
            success = false;
        }//ktra 
        //tạo đối tượng font chữ menu
        font_MENU = TTF_OpenFont("font//dlxfont.ttf", 50);
        if (font_MENU == NULL)//ktra
        {
            success = false;
        }
        //khởi tạo âm thanh tần số 22050, chế độ mix_default, 2 tai trái phải
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)//lỗi
        {
            success = false;
        }
    }


    //đọc file wav audio
    g_sound_bullet[0] = Mix_LoadWAV("sound//bullet.wav");
    

    g_sound_exp[0] = Mix_LoadWAV("sound//explosion.wav");
    g_sound_exp[1] = Mix_LoadWAV("sound//explosion2.wav");

    g_sound[0] = Mix_LoadWAV("sound//game_audio.wav");
    g_sound[1] = Mix_LoadWAV("sound//jump_audio.wav");
   
    
    //kiểm tra xem có đọc đc ko
    if (g_sound_exp[0] == NULL || g_sound_bullet[0] == NULL || g_sound_exp[1] == NULL || g_sound[0] == NULL || g_sound[1] == NULL)
    {
        success = false;
    }
    return success;
}
bool LoadBackground()
{
    bool ret = g_background.LoadImg("img//background.png", g_screen);
    if (ret == false) return false;
    return true;
}

void close() // hàm giải phóng
{
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;
    SDL_DestroyWindow(g_window);
    g_window = NULL;
    IMG_Quit();
    SDL_Quit();
} // kết thúc chương trình


//gọi ra threats động
std::vector<ThreatsObject*> MakeThreatList()//tạo 1 list threat
{
    std::vector<ThreatsObject*> list_threats;
    ThreatsObject* dynamic_threats = new ThreatsObject[20];//tạo ra 20 đối tượng threat trên bản đồ
    for (int i = 0; i < 20; i++)
    {
        ThreatsObject* p_threat = (dynamic_threats + i);//lựa từng cái 1
        if (p_threat != NULL)
        {
            p_threat->LoadImg("img//threat_left.png", g_screen);//load ảnh threat lên
            p_threat->set_clips();//tạo kích thước cho từng frame
            p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
            p_threat->set_x_pos(500 + i * 500);
            p_threat->set_y_pos(200);
            //tính pos1,2 dựa trên x_pos đang có
            int pos1 = p_threat->get_x_pos() - 60;
            int pos2 = p_threat->get_x_pos() + 60;
            p_threat->SetAnimationPos(pos1, pos2);
            p_threat->set_input_left(1);
            list_threats.push_back(p_threat);//đưa vào list threat
        }
    }
    //gọi ra threats tĩnh
    ThreatsObject* threats_objs = new ThreatsObject[20];

    for (int i = 0; i < 20; i++)
    {
        ThreatsObject* p_threat = (threats_objs + i);
        if (p_threat != NULL)
        {
            p_threat->LoadImg("img//threat_level.png", g_screen);
            p_threat->set_clips();
            p_threat->set_x_pos(700 + i*1200);
            p_threat->set_y_pos(250);
            p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
            p_threat->set_input_left(0);//trạng thái không di chuyển

            //tạo ra 1 lớp đạn
            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, g_screen);//khởi tạo viên đạn
            list_threats.push_back(p_threat);//đưa vào list


        }
    }
    return list_threats;//list threat chung
}



int main(int argc, char* argv[])
{
    ImpTimer fps_timer;
    bool is_quit = false;


    if (InitData() == false) return -1;

    if (LoadBackground() == false) return -1;

    Mix_PlayChannel(-1, g_sound[0], 4);//âm thanh toàn bộ game

    //Show menu khi bắt đầu game
    int ret_menu = SDLCommonFunc::ShowMenu(g_screen, font_MENU, "Play", "Exit", "img//menu.png");
    if (ret_menu == 1)// Khi bấm exit
    {
        is_quit = true;
    }

    

again:
    //gọi map
    GameMap game_map;
    char gm[] = "map//map.dat";
    game_map.LoadMap(gm);
    game_map.LoadTiles(g_screen);



    //khai báo hàm nhân vật 
    MainPlayerObject p_player;
    p_player.LoadImg("img//player_right.png", g_screen);
    p_player.set_clips();//hiệu ứng

    //khai báo hàm sinh mạng cho nvat chính
    PlayerPower player_power;
    player_power.Init(g_screen);//khởi tạo 3 mạng, load ảnh

    //khai báo hàm load tấm ảnh tiền
    PlayerMoney player_money;
    player_money.Init(g_screen);
    player_money.SetPos(SCREEN_WIDTH * 0.5 - 300, 8);

    //khai báo hàm load tấm ảnh trứng
    PlayerEgg player_egg;
    player_egg.Init(g_screen);
    player_egg.SetPos(SCREEN_WIDTH * 0.5 - 100, 8);

    PlayerDiamond player_diamond;
    player_diamond.Init(g_screen);
    player_diamond.SetPos(SCREEN_WIDTH * 0.5 - 450, 8);

    //khai báo threats
    std::vector<ThreatsObject*> threats_list = MakeThreatList();



    //Hiệu ứng vụ nổ viên đạn bắn vào quái vật
    ExplosionObject exp_threat;
    bool tRet = exp_threat.LoadImg("img//exp3.png", g_screen);//load ảnh
    if (!tRet) return -1;
    exp_threat.set_clips();


    ExplosionObject exp_main;
    bool mRet = exp_main.LoadImg("img//exp3.png", g_screen);
    exp_main.set_clips();
    if (!mRet) return -1;




    //biến quản lí sinh mạng
    int num_die = 0;

    //Time text
    TextObject time_game;
    time_game.SetColor(TextObject::WHITE_TEXT);//set màu

    //mark text 
    TextObject mark_game;
    mark_game.SetColor(TextObject::WHITE_TEXT);
    UINT mark_value = 0;//biến quản lí số mark

    //money text
    TextObject money_game;
    money_game.SetColor(TextObject::WHITE_TEXT);


    //egg text
    TextObject egg_game;
    egg_game.SetColor(TextObject::WHITE_TEXT);


    TextObject diamond_game;
    diamond_game.SetColor(TextObject::WHITE_TEXT);

    while (!is_quit)
    {
        fps_timer.start();//bắt đầu thời gian
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT) is_quit = true; // tắt chương trình
            p_player.HandleInputAction(g_event, g_screen, g_sound_bullet);
        }
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        // set lại màu cho màn hình
        SDL_RenderClear(g_screen); // làm mới lại màn hình

        g_background.Render(g_screen, NULL); // đưa ảnh vào màn hình

        Map map_data = game_map.getMap();

        p_player.HandleBullet(g_screen);//gọi đạn bắn
        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);//cập nhật vị trí
        p_player.DoPlayer(map_data, g_sound);
        p_player.Show(g_screen);



        game_map.SetMap(map_data);//Bản đồ cập nhật vị trí mới
        game_map.DrawMap(g_screen);//Vẽ lại


        //DrawGeometric
        GeometricFormat rectangle_size(0, 0, SCREEN_WIDTH, 40);//vẽ hình chữ nhật
        ColorData color_data(36, 36, 36);//màu sắc hcn dựa trên paint
        Geometric::RenderRectangle(rectangle_size, color_data, g_screen);//show ra màn hình

        GeometricFormat outLineSize(1, 1, SCREEN_WIDTH - 1, 38);//vẽ viền
        ColorData color_data2(255, 255, 255);//màu viền
        Geometric::RenderOutline(outLineSize, color_data2, g_screen);//show viền ra màn hình



        //show ảnh sinh mạng, tiền, trứng 
        player_power.Show(g_screen);
        player_money.Show(g_screen);
        player_egg.Show(g_screen);
        player_diamond.Show(g_screen);

        //Xuất hiện threats
        for (int i = 0; i < threats_list.size(); i++)
        {
            ThreatsObject* p_threat = threats_list.at(i);//Lấy threat ra
            if (p_threat != NULL)
            {
                p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
                p_threat->ImpMoveType(g_screen);
                p_threat->DoPlayer(map_data);
                //đưa đạn vào
                p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
                p_threat->Show(g_screen);


                SDL_Rect rect_player = p_player.GetRectFrame();//lấy rect của nvat game 
                //kiểm tra threat bullet với nvat main
                bool bCol1 = false;
                std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();//lấy dsach bullet threat 
                for (int j = 0; j < tBullet_list.size(); ++j)
                {
                    BulletObject* pt_bullet = tBullet_list.at(j);//lấy từng bullet threat 1
                    if (pt_bullet)
                    {
                        bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);//check va chạm
                        if (bCol1)
                        {
                            p_threat->RemoveBullet(j);//xóa đạn
                            break;//không kiểm tra với đạn khác nữa
                        }
                    }
                }
                //kiểm tra va chạm main và threat
                SDL_Rect rect_threat = p_threat->GetRectFrame();
                bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);//check va chạm
                if (bCol1 || bCol2)//xảy ra 1 trong 2 va chạm
                {
                    int width_exp_frame = exp_main.get_frame_width();
                    int height_exp_frame = exp_main.get_frame_height();
                    for (int ex = 0; ex < 4; ex++)
                    {
                        int x_pos = (p_player.GetRect().x + p_player.get_frame_width() * 0.5) - width_exp_frame * 0.5;
                        int y_pos = (p_player.GetRect().y + p_player.get_frame_height() * 0.5) - height_exp_frame * 0.5;

                        exp_main.set_frame(ex);
                        exp_main.SetRect(x_pos, y_pos);
                        exp_main.Show(g_screen);
                        SDL_RenderPresent(g_screen);

                    }


                    Mix_PlayChannel(-1, g_sound_exp[1], 0);//âm thanh va chạm xuất hiện
                    //chỉ số sinh mạng
                    num_die++;
                    if (num_die < 3)
                    {
                        p_player.SetRect(0, 0);//set lại rect cho nvat main ở nơi xuất phát
                        p_player.set_comeback_time(60);//set lại thời gian nvat quay lại 
                        SDL_Delay(1000);//tgian delay 1s
                        player_power.Decrease();//giảm mạng
                        player_power.Render(g_screen);//load lại mạng 
                        continue;
                    }
                    else
                    {
                        Sleep(500);
                        int ret_menu = SDLCommonFunc::ShowMenu(g_screen, font_MENU, "Play Again", "Exit", "img//menu_again.png");
                        if (ret_menu == 1)
                        {
                            is_quit = true;
                            break;
                        }
                        else if (ret_menu == 0)
                        {
                            is_quit = false;
                            goto again;
                        }

                    }
                }

            }
        }

        //vị trí chính xác của vụ nổ
        int frame_exp_width = exp_threat.get_frame_width();
        int frame_exp_height = exp_threat.get_frame_height();
        //danh sách các viên đạn của nhân vật game
        std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
        for (int r = 0; r < bullet_arr.size(); ++r)
        {//kiểm tra
            BulletObject* p_bullet = bullet_arr.at(r);//lấy từng viên đạn ra
            if (p_bullet != NULL)
            {
                for (int t = 0; t < threats_list.size(); ++t)
                {//kiểm tra từng viên đạn với từng đối tượng threat
                    ThreatsObject* obj_threat = threats_list.at(t);//lấy từng threat ra
                    if (obj_threat != NULL)
                    {
                        SDL_Rect tRect;// lấy rect threat
                        tRect.x = obj_threat->GetRect().x;
                        tRect.y = obj_threat->GetRect().y;
                        tRect.w = obj_threat->get_width_frame();//vì sử dụng khung hình frame để load hiệu ứng động nên chỉ lấy 1 frame
                        tRect.h = obj_threat->get_height_frame();//viên đạn chưa chạm tới ảnh nvat đã có hiệu ứng

                        SDL_Rect bRect = p_bullet->GetRect();//lấy rect bullet

                        bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);//kiểm tra va chạm giữa đạn của nvat và threat

                        if (bCol)//Nếu có va chạm
                        {
                            mark_value++;//cộng thêm 1 mark khi bắn chết
                            for (int ex = 0; ex < NUM_FRAME_EXP; ++ex)
                            {
                                int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;//vị trí đặt tấm ảnh vụ nổ vào, là vị trí viên đạn chạm vào
                                int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.5;


                                exp_threat.set_frame(ex);//đang ở frame bao nhiêu
                                exp_threat.SetRect(x_pos, y_pos);//set rect cho vụ nổ
                                exp_threat.Show(g_screen);//show ra màn hình


                            }

                            p_player.RemoveBullet(r);//xóa đạn 
                            obj_threat->Free();//hủy threat
                            threats_list.erase(threats_list.begin() + t);//loại khỏi danh sách threat
                            Mix_PlayChannel(-1, g_sound_exp[0], 0);//âm thanh va chạm 
                        }
                    }
                }
            }
        }

        //Show game time
        std::string str_time = "Time: ";//Hiện ra màn hình
        Uint32 time_val = SDL_GetTicks() / 1000;//thời điểm hiện tại chia 1000 để ra s
        Uint32 val_time = 270 - time_val;//đếm ngược
        if (val_time <= 0)//hết giờ
        {
            Sleep(500);
            int ret_menu = SDLCommonFunc::ShowMenuEnd(g_screen, font_MENU, "Exit", "img//menu_lose.png");
            if (ret_menu == 0)
            {
                is_quit = true;
            }
        }
        else//show text time lên
        {
            std::string str_val = std::to_string(val_time);
            str_time += str_val;//time kèm tgian 

            time_game.SetText(str_time);//set text
            time_game.LoadFromRenderText(font_time, g_screen);//show ra màn hình
            time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);//vị trí show
        }


        //show text mark: giống hệt time
        std::string val_str_mark = std::to_string(mark_value);
        std::string strMark("Mark: ");
        strMark += val_str_mark;

        mark_game.SetText(strMark);
        mark_game.LoadFromRenderText(font_time, g_screen);
        mark_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 + 200, 15);


        //show text money
        int money_count = p_player.GetMoneyCount();//tiền ăn được hiện tại
        std::string money_str = std::to_string(money_count);//chuyển đổi về string để set text
        money_game.SetText(money_str);
        money_game.LoadFromRenderText(font_time, g_screen);
        money_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 250, 15);

        int egg_count = p_player.GetEggCount();
        std::string egg_str = std::to_string(egg_count);
        egg_game.SetText(egg_str);
        egg_game.LoadFromRenderText(font_time, g_screen);
        egg_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 50, 15);


        int diamond_count = p_player.GetDiaCount();
        std::string dia_str = std::to_string(diamond_count);
        diamond_game.SetText(dia_str);
        diamond_game.LoadFromRenderText(font_time, g_screen);
        diamond_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 400, 15);


        //win
        if (egg_count == 3)
        {
            int ret_menu = SDLCommonFunc::ShowMenuEnd(g_screen, font_MENU, "Exit", "img//menu_win.png");
            if (ret_menu == 0)
            {
                is_quit = true;
            }
        }


        SDL_RenderPresent(g_screen);
            
 

        //xử lí thời gian
        int real_imp_time = fps_timer.get_ticks();//thời gian đã trôi qua
        int time_one_frame = 1000 / FRAME_PER_SECOND; // ms

        if (real_imp_time < time_one_frame)//thời gian thực tế chưa đến lý thuyết 
        {
            int delay_time = time_one_frame - real_imp_time;
            if (delay_time >= 0) SDL_Delay(delay_time);//độ trễ
        }
    }
        //hủy bỏ threats đi khi kết thúc chương trình
        for (int i = 0; i < threats_list.size(); i++)
        {
            ThreatsObject* p_threat = threats_list.at(i);
            if (p_threat != NULL)
            {
                p_threat->Free();//hủy bỏ con trỏ p_object bên trong
                p_threat = NULL;

            }
        }

        threats_list.clear();//xóa toàn bộ list

        close();
        return 0;
    
}