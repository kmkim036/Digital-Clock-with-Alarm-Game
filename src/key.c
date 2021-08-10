#include <key.h>

void key0(int sw)
{
    if (sw == 1)
        mode = 1;
    else if (sw == 2)
        mode = 2;
    else if (sw == 4)
        mode = 3;
    else if (sw == 8)
        mode = 4;
    else if (sw == 16)
        mode = 5;
    else if (sw == 32)
        mode = 6;
    else
        mode = 1;

    submode = 1;
    count2 = 0;
    run_SW = 0;
    run_timer = 0;
    lapcount = 0;
    sw_c = 0;

    for (int i = 0; i < 5; i++)
        laptime[i] = 0;
}

void key1()
{
    if (mode == 1) {
        if (submode == 1)
            ;
        else if (submode == 2)
            count1 = (count1 + 3600) % 86400;
        else if (submode == 3)
            count1 = (count1 + 60) % 86400;
        else if (submode == 4)
            count1++;
        count1 = (count1 % 86400);
    } else if (mode == 2) {
        if (submode == 1)
            ;
        else if (submode == 2)
            year = (year % 100) + 1;
        else if (submode == 3)
            month = (month % 12) + 1;
        else if (submode == 4) {
            if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
                day = (day % 31) + 1;
            else if (month == 4 || month == 6 || month == 9 || month == 11)
                day = (day % 30) + 1;
            else if (month == 2 && (year % 4 == 0))
                day = (day % 29) + 1;
            else
                day = (day % 28) + 1;
        }
    } else if (mode == 3) {
        submode = ((submode + 4) % 6) + 1;
    } else if (mode == 4) {
        count2 = 0;
        for (int j = 0; j < lapcount; j++)
            laptime[j] = 0;
        lapcount = 0;
        sw_c = 0;
    } else if (mode == 5) {
        if (submode == 1)
            count2 = (count2 + 600) % 60000;
        else if (submode == 2)
            count2 = (count2 + 10) % 60000;
    } else if (mode == 6) {
        if (submode == 1)
            alarm_time = (alarm_time + 3600) % 86400;
        else if (submode == 2)
            alarm_time = (alarm_time + 60) % 86400;
        else if (submode == 3)
            alarm_time++;
        alarm_time = (alarm_time % 86400);
    } else {
        man_x += 5;
        if (man_x > 315)
            man_x = 315;
    }
}

void key2()
{
    if (mode == 1)
        submode = (submode % 4) + 1;
    else if (mode == 2)
        submode = (submode % 4) + 1;
    else if (mode == 3)
        submode = (submode % 6) + 1;
    else if (mode == 4) {
        if (run_SW)
            run_SW = 0;
        else
            run_SW = 1;
    } else if (mode == 5) {
        if (submode == 3) {
            if (run_timer)
                run_timer = 0;
            else
                run_timer = 1;
        } else
            submode++;
    } else if (mode == 6) {
        submode = (submode % 3) + 1;
    } else {
        man_x -= 5;
        if (man_x < 5)
            man_x = 5;
    }
}

void key3()
{
    if (mode == 4) {
        laptime[lapcount] = count2;
        lapcount = (lapcount + 1) % 5;
        sw_c++;
    } else if (mode == 7) {
        bullet_x = man_x;
        bullet_y = 239;
        dy = -3;
    }
}