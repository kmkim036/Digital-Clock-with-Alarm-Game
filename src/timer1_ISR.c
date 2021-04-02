#include "address_map_nios2.h"

static volatile int* TIMER1_ptr = (int*)TIMER_BASE;
extern int count1;
extern int count1_world;

extern int day;
extern int year;
extern int month;
extern int count1_1;
extern int count2;
extern int mode;
extern int run_SW;
extern int run_timer;

void timer1_ISR()
{
    (*TIMER1_ptr) = 0;
    count1_1++;
    if (count1_1 == 10) {
        count1_1 = 0;
        count1++;
        count1_world = (count1_world % 86400) + 1;
        if (count1 == 86400) {
            count1 = 0;
            day++;
            if (day == 32 && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)) {
                day = 1;
                month++;
            } else if (day == 31 && (month == 4 || month == 6 || month == 9 || month == 11)) {
                day = 1;
                month++;
            } else if (day == 30 && month == 2 && ((year % 4) == 0)) {
                day = 1;
                month++;
            } else if (day == 29 && month == 2 && (year % 4) != 0) {
                day = 1;
                month++;
            }
            if (month == 13) {
                year++;
                month = 1;
            }
        }
    }
    if (mode == 4 && run_SW) {
        count2 = (count2 + 1) % 60000;
    }
    if (mode == 5 && run_timer) {
        if (count2 > 0)
            count2--;
    }

    return;
}