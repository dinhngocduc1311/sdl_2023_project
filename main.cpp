
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
#include "BossObject.h"
#include <Windows.h>
#include <WinUser.h>



BaseObject g_background;
TTF_Font* font_time = NULL;
TTF_Font* font_MENU = NULL;



bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    g_window = SDL_CreateWindow("22028166",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SCREEN_WIDTH, 
                                 SCREEN_HEIGHT, 
                                 SDL_WINDOW_SHOWN);
    if (g_window == NULL) success = false;
    
    else 
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL) success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags)) success = false;
        }

        if (TTF_Init() == -1)
        {
            success = false;
        }
        font_time = TTF_OpenFont("font//dlxfont.ttf", 15);
        if (font_time == NULL)
        {
            success = false;
        }
        font_MENU = TTF_OpenFont("font//dlxfont.ttf", 50);
        if (font_MENU == NULL)
        {
            success = false;
        }
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
            success = false;
        }
    }


    //Read file wav audio
    g_sound_bullet[0] = Mix_LoadWAV("sound//bullet.wav");
    

    g_sound_exp[0] = Mix_LoadWAV("sound//explosion.wav");
    g_sound_exp[1] = Mix_LoadWAV("sound//explosion2.wav");

    g_sound[0] = Mix_LoadWAV("sound//game_audio.wav");
    g_sound[1] = Mix_LoadWAV("sound//jump_audio.wav");
   
    

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

void close()
{
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;
    SDL_DestroyWindow(g_window);
    g_window = NULL;
    IMG_Quit();
    SDL_Quit();
}

std::vector<ThreatsObject*> MakeThreatList()
{
    std::vector<ThreatsObject*> list_threats;
    ThreatsObject* dynamic_threats = new ThreatsObject[20];
    for (int i = 0; i < 20; i++)
    {
        ThreatsObject* p_threat = (dynamic_threats + i);
        if (p_threat != NULL)
        {
            p_threat->LoadImg("img//threat_left.png", g_screen);
            p_threat->set_clips();
            p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
            p_threat->set_x_pos(500 + i * 500);
            p_threat->set_y_pos(200);
            int pos1 = p_threat->get_x_pos() - 60;
            int pos2 = p_threat->get_x_pos() + 60;
            p_threat->SetAnimationPos(pos1, pos2);
            p_threat->set_input_left(1);
            list_threats.push_back(p_threat);
        }
    }
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
            p_threat->set_input_left(0);

            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, g_screen);
            list_threats.push_back(p_threat);


        }
    }
    return list_threats;
}

int main(int argc, char* argv[])
{

    ImpTimer fps_timer;
    bool is_quit = false;


    if (InitData() == false) return -1;



    Mix_PlayChannel(-1, g_sound[0], 4);



    //Make menu game
    int ret_menu = SDLCommonFunc::ShowMenu(g_screen, font_MENU, "Play Game", "Exit", "img//Menu.png");
    if (ret_menu == 1)
    {
        is_quit = true;
    }



    if (LoadBackground() == false) return -1;



again_label:


    GameMap game_map;
    char gm[] = "map//map.dat";
    game_map.LoadMap(gm);
    game_map.LoadTiles(g_screen);


    

    MainPlayerObject p_player;
    p_player.LoadImg("img//player_right.png", g_screen);
    p_player.set_clips();


    PlayerPower player_power;
    player_power.Init(g_screen);


    PlayerMoney player_money;
    player_money.Init(g_screen);
    player_money.SetPos(SCREEN_WIDTH * 0.5 - 300, 8);

    PlayerEgg player_egg;
    player_egg.Init(g_screen);
    player_egg.SetPos(SCREEN_WIDTH * 0.5 - 100, 8);


    std::vector<ThreatsObject*> threats_list = MakeThreatList();

    //Boss Threat
    BossObject bossObject;
    bool ret = bossObject.LoadImg("img//boss_object.png", g_screen);
    bossObject.set_clips();
    int xPosBoss = MAX_MAP_X*TILE_SIZE - SCREEN_WIDTH*0.6;
    bossObject.set_x_pos(xPosBoss);
    bossObject.set_y_pos(10);


    ExplosionObject exp_threat;
    bool tRet = exp_threat.LoadImg("img//exp3.png", g_screen);
    if (!tRet) return -1;
    exp_threat.set_clips();


    ExplosionObject exp_main;
    bool mRet = exp_main.LoadImg("img//exp3.png", g_screen);
    exp_main.set_clips();
    if (!mRet) return -1;
    

    int num_die = 0;

    //Time text
    TextObject time_game;
    time_game.SetColor(TextObject::WHITE_TEXT);

    TextObject mark_game;
    mark_game.SetColor(TextObject::WHITE_TEXT);
    UINT mark_value = 0;

    TextObject money_game;
    money_game.SetColor(TextObject::WHITE_TEXT);

    TextObject egg_game;
    egg_game.SetColor(TextObject::WHITE_TEXT);
    
    
   
    while (!is_quit)
    {
        fps_timer.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT) is_quit = true;
            p_player.HandleInputAction(g_event, g_screen, g_sound_bullet);
        }
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        
        SDL_RenderClear(g_screen);
        
        g_background.Render(g_screen, NULL);
        
        Map map_data = game_map.getMap();
        
        p_player.HandleBullet(g_screen);
        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
        p_player.DoPlayer(map_data, g_sound);
        p_player.Show(g_screen);

        game_map.SetMap(map_data);
        game_map.DrawMap(g_screen);


        //DrawGeometric
        GeometricFormat rectangle_size(0, 0, SCREEN_WIDTH, 40);
        ColorData color_data(36,36,36);
        Geometric::RenderRectangle(rectangle_size, color_data, g_screen);

        GeometricFormat outLineSize(1, 1, SCREEN_WIDTH - 1, 38);
        ColorData color_data2(255, 255, 255);

        Geometric::RenderOutline(outLineSize, color_data2, g_screen);




        player_power.Show(g_screen);
        player_money.Show(g_screen);
        player_egg.Show(g_screen);

        
        for (int i = 0; i < threats_list.size(); i++)
        {
            ThreatsObject* p_threat = threats_list.at(i);
            if (p_threat != NULL)
            {
                p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
                p_threat->ImpMoveType(g_screen);
                p_threat->DoPlayer(map_data);
                p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
                p_threat->Show(g_screen);


                SDL_Rect rect_player = p_player.GetRectFrame();
                bool bCol1 = false;
                std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
                for (int j = 0; j < tBullet_list.size(); ++j)
                {
                    BulletObject* pt_bullet = tBullet_list.at(j);
                    if (pt_bullet)
                    {
                        bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
                        if (bCol1)
                        {
                            p_threat->RemoveBullet(j);
                            break;
                        }
                    }
                }

                SDL_Rect rect_threat = p_threat->GetRectFrame();
                bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);
                if (bCol1 || bCol2)
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
                    Mix_PlayChannel(-1, g_sound_exp[1], 0);
                    num_die++;
                    if (num_die < 3)
                    {
                        p_player.SetRect(0, 0);
                        p_player.set_comeback_time(60);
                        SDL_Delay(1000);
                        player_power.Decrease();
                        player_power.Render(g_screen);
                        continue;
                    }
                    else
                    {
                        Sleep(500);
                        int ret_menu = SDLCommonFunc::ShowMenu(g_screen, font_MENU, "Play Again", "Exit", "img//Menu_again.png");
                        if (ret_menu == 1)
                        {
                            is_quit = true;
                        }
                        else
                        {
                            is_quit = false;
                            goto again_label;
                        }
                    }
                    
                    
                }

            }
        }


        int frame_exp_width = exp_threat.get_frame_width();
        int frame_exp_height = exp_threat.get_frame_height();
        std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
        for (int r = 0; r < bullet_arr.size(); ++r)
        {
            BulletObject* p_bullet = bullet_arr.at(r);
            if (p_bullet != NULL)
            {
                for (int t = 0; t < threats_list.size(); ++t)
                {
                    ThreatsObject* obj_threat = threats_list.at(t);
                    if (obj_threat != NULL)
                    {
                        SDL_Rect tRect;
                        tRect.x = obj_threat->GetRect().x;
                        tRect.y = obj_threat->GetRect().y;
                        tRect.w = obj_threat->get_width_frame();
                        tRect.h = obj_threat->get_height_frame();

                        SDL_Rect bRect = p_bullet->GetRect();
                        
                        bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);
                        
                        if (bCol)
                        {
                            mark_value++;
                            for (int ex = 0; ex < NUM_FRAME_EXP; ++ex)
                            {
                                int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
                                int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.5;


                                exp_threat.set_frame(ex);
                                exp_threat.SetRect(x_pos, y_pos);
                                exp_threat.Show(g_screen);


                            }

                            p_player.RemoveBullet(r);
                            obj_threat->Free();
                            threats_list.erase(threats_list.begin() + t);
                            Mix_PlayChannel(-1, g_sound_exp[0], 0);
                        }
                    }
                }
            }
        }

        //Show game time
        std::string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks() / 1000;
        Uint32 val_time = 270 - time_val;
        if (val_time <= 0)
        {
            Sleep(500);
            int ret_menu = SDLCommonFunc::ShowMenuEnd(g_screen, font_MENU, "Exit", "img//Menu_lose.png");
            if (ret_menu == 0)
            {
                is_quit = true;
            }
        }
        else
        {
            std::string str_val = std::to_string(val_time);
            str_time += str_val;

            time_game.SetText(str_time);
            time_game.LoadFromRenderText(font_time, g_screen);
            time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
        }



        std::string val_str_mark = std::to_string(mark_value);
        std::string strMark("Mark: ");
        strMark += val_str_mark;

        mark_game.SetText(strMark);
        mark_game.LoadFromRenderText(font_time, g_screen);
        mark_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 + 200, 15);

        int money_count = p_player.GetMoneyCount();
        std::string money_str = std::to_string(money_count);
        money_game.SetText(money_str);
        money_game.LoadFromRenderText(font_time, g_screen);
        money_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 250, 15);

        int egg_count = p_player.GetEggCount();
        std::string egg_str = std::to_string(egg_count);
        egg_game.SetText(egg_str);
        egg_game.LoadFromRenderText(font_time, g_screen);
        egg_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 50, 15);

        //win
        if (egg_count == 3)
        {
            int ret_menu = SDLCommonFunc::ShowMenuEnd(g_screen, font_MENU, "Exit", "img//Menu_win.png");
            if (ret_menu == 0)
            {
                is_quit = true;
            }
        }

        //Show Boss
        int val = MAX_MAP_X * TILE_SIZE - (map_data.start_x_ + p_player.GetRect().x);
        if (val <= SCREEN_WIDTH)
        {
            bossObject.SetMapXY(map_data.start_x_, map_data.start_y_);
            bossObject.DoPlayer(map_data);
            bossObject.MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
            bossObject.Show(g_screen);
        }
        


        SDL_RenderPresent(g_screen);

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000 / FRAME_PER_SECOND; // ms

        if (real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            if (delay_time>=0) SDL_Delay(delay_time);
                
        }
    }

    for (int i = 0; i < threats_list.size(); i++)
    {
        ThreatsObject* p_threat = threats_list.at(i);
        if (p_threat != NULL)
        {
            p_threat->Free();
            p_threat = NULL;

        }
    }

    threats_list.clear();


    

    close();
    return 0;
}