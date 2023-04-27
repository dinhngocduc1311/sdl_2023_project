
#include "ImpTimer.h"
#include "CommonFunc.h"
ImpTimer::ImpTimer()
{
	start_tick_ = 0;
	is_started_ = false;

}

ImpTimer::~ImpTimer()
{

}

void ImpTimer::start()
{
	is_started_ = true;
	start_tick_ = SDL_GetTicks();//Liên tục lấy thời gian đã trôi qua

}

void ImpTimer::stop()
{
	is_started_ = false;
}


int ImpTimer::get_ticks() //thời điểm tồn tại đang là gì
{
	if (is_started_ == true)
	{
		return SDL_GetTicks() - start_tick_;
	}
	return 0;
}


