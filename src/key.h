#include "address_map_nios2.h"

static volatile int* TIMER2_ptr = (int*)TIMER_2_BASE;

extern int mode;
extern int submode;

extern int count1;
extern int count2;

extern int day;
extern int year;
extern int month;

extern int alarm;
extern int alarm_time;

extern int run_SW;
extern int sw_c;

extern int run_timer;
extern int laptime[5];
extern int lapcount;

extern int dy;
extern int bullet_x;
extern int bullet_y;
extern int man_x;

void key0(int sw);
void key1();
void key2();
void key3();