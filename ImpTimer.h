
#ifndef IMP_TIMER_H_
#define IMP_TIMER_H_

// xử lý timer và tỉ số fps game
class ImpTimer
{
public:
	ImpTimer();
	~ImpTimer();

	void start();
	void stop();
	int get_ticks(); 

private:
	int start_tick_; //thời điểm bắt đầu chạy 
	bool is_started_;
};





#endif
