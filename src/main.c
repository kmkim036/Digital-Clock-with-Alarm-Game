#include "address_map_nios2.h"
#include "monitor.h"
#include "nios2_ctrl_reg_macros.h"

//memory mapped I/O
static volatile int* KEY_ptr = (int*)KEY_BASE;
static volatile int* SW_ptr = (int*)SW_BASE;
static volatile int* TIMER1_ptr = (int*)TIMER_BASE;
static volatile int* LEDR_ptr = (int*)LEDR_BASE;
static volatile int* HEX3_HEX0_ptr = (int*)HEX3_HEX0_BASE;
static volatile int* HEX5_HEX4_ptr = (int*)HEX5_HEX4_BASE;

//global variable
int mode = 1;
int submode = 1;

int count1 = 35120; //counts for 1sec, initialize 9:45:20
int count2 = 0; //counts for 0.1sec (stopwatch,timer)
int count1_world;
int count1_1 = 0;

//default
int year = 18;
int month = 12;
int day = 24;

int sw_c = 0;
int laptime[5];
int lapcount = 0;

int run_timer = 0;
int run_SW = 0;

//initialize 9:45:00
int alarm_time = 35100;

int x = 160;
int y = 50;
int dx = 2;
int ufo_x = 160;
int bullet_x = 160;
int bullet_y = 240;
int dy = 0;
int hp = 3;
int man_x = 160;

void config_KEYS()
{
    *(KEY_ptr + 2) = 15;
}

void config_TIMER1()
{
    int i, j;
    i = 10000000 - 1;
    j = i >> 16;
    *(TIMER1_ptr + 2) = i;
    *(TIMER1_ptr + 3) = j;
    *(TIMER1_ptr + 1) = 0b0111; //set cont, start, ITO
}

void enable_nios2_interrupts()
{
    NIOS2_WRITE_IENABLE(0b11);
    NIOS2_WRITE_STATUS(1); //enable NIOS 2 interrupt
}

int seg7_code(int i)
{
    char code[10] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111 };
    return code[i];
}

void hex3_hex0_mode1_3_6(int b)
{
    int i, j;
    int a = b % 3600;

    i = seg7_code(a / 600);
    j = i << 24;
    a = a % 600;
    i = seg7_code(a / 60);
    i = i << 16;
    j = j | i;
    a = a % 60;
    i = seg7_code(a / 10);
    i = i << 8;
    j = j | i;
    a = a % 10;
    i = seg7_code(a);
    j = j | i;
    *HEX3_HEX0_ptr = j;
}

void hex5_hex4_mode1_3_6(int b)
{
    int i, j;
    int a = b / 3600;
    i = seg7_code(a / 10);
    i = i << 8;
    j = seg7_code(a % 10);
    j = j | i;
    *HEX5_HEX4_ptr = j;
}

void hex3_hex0_mode2(int b)
{
    int i, j, a = b;
    i = seg7_code(a / 1000);
    j = i << 24;
    a = a % 1000;
    i = seg7_code(a / 100);
    i = i << 16;
    j = j | i;
    a = a % 100;
    i = seg7_code(a / 10);
    i = i << 8;
    j = j | i;
    a = a % 10;
    i = seg7_code(a);
    j = j | i;
    *HEX3_HEX0_ptr = j;
}

void hex5_hex4_mode2(int b)
{
    int i, j;
    int a = b;
    i = seg7_code(a / 10);
    j = i << 8;
    a = a % 10;
    i = seg7_code(a);
    j = j | i;
    *HEX5_HEX4_ptr = j;
}

void hex3_hex0_mode4_5(int b)
{
    int i, j;
    int a = b % 600;
    i = seg7_code(a / 100);
    j = i << 24;
    a = a % 100;
    i = seg7_code(a / 10);
    i = i << 16;
    j = j | i;
    i = 0b100000000000;
    j = j | i;
    a = a % 10;
    i = seg7_code(a);
    j = j | i;
    *HEX3_HEX0_ptr = j;
}

void hex5_hex4_mode4_5(int b)
{
    int i, j, a = b / 600;
    i = seg7_code(a / 10);
    i = i << 8;
    j = seg7_code(a % 10);
    j = j | i;
    *HEX5_HEX4_ptr = j;
}

void main()
{

    config_TIMER1();
    config_KEYS();
    enable_nios2_interrupts();

    pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;
    *(pixel_ctrl_ptr + 1) = front_buffer; //first store the address in the back buffer
    wait_for_vsync();
    pixel_buffer_start = *pixel_ctrl_ptr; //set a pointer to the pixel buffer
    clear_screen();
    *(pixel_ctrl_ptr + 1) = back_buffer; //set a location for the back pixel buffer

    while (1) //HEX display
    {
        config_KEYS();
        *LEDR_ptr = mode;

        if (alarm_time == count1) {
            mode = 7;
            hp = 3;
        }

        if (mode == 1) {
            hex3_hex0_mode1_3_6(count1);
            hex5_hex4_mode1_3_6(count1);
            int s, ss, m, mm, h, hh;
            s = count1 % 10;
            ss = (count1 % 60) / 10;
            m = (count1 % 600) / 60;
            mm = (count1 % 3600) / 600;
            h = (count1 / 3600) % 10;
            hh = (count1 / 36000);
            DISPLAY(s, ss, m, mm, h, hh);
        } else if (mode == 2) {
            hex3_hex0_mode2(month * 100 + day);
            hex5_hex4_mode2(year);
            int d, dd, m, mm, y, yy;
            d = day % 10;
            dd = day / 10;
            m = month % 10;
            mm = month / 10;
            y = year % 10;
            yy = year / 10;
            DISPLAY(d, dd, m, mm, y, yy);
        } else if (mode == 3) {
            switch (submode) {
            case 1:
                count1_world = count1;
                break; //Seoul
            case 2:
                count1_world = (count1 + 23 * 3600) % 86400;
                break; //Beijing
            case 3:
                count1_world = (count1 + 18 * 3600) % 86400;
                break; //Moscow
            case 4:
                count1_world = (count1 + 16 * 3600) % 86400;
                break; //Rome
            case 5:
                count1_world = (count1 + 10 * 3600) % 86400;
                break; //NY
            case 6:
                count1_world = (count1 + 7 * 3600) % 86400;
                break; //LA
            }
            hex3_hex0_mode1_3_6(count1_world);
            hex5_hex4_mode1_3_6(count1_world);
            int s, ss, m, mm, h, hh;
            s = count1_world % 10;
            ss = (count1_world % 60) / 10;
            m = (count1_world % 600) / 60;
            mm = (count1_world % 3600) / 600;
            h = (count1_world / 3600) % 10;
            hh = (count1_world / 36000);
            DISPLAY(s, ss, m, mm, h, hh);
        } else if (mode == 4) {
            hex3_hex0_mode4_5(count2);
            hex5_hex4_mode4_5(count2);
            int t, s, ss, m, mm;
            t = count2 % 10;
            s = (count2 % 100) / 10;
            ss = (count2 % 600) / 100;
            m = (count2 / 600) % 10;
            mm = (count2 / 6000);
            DISPLAY(t, 100, s, ss, m, mm);
        } else if (mode == 5) {
            hex3_hex0_mode4_5(count2);
            hex5_hex4_mode4_5(count2);
            int t, s, ss, m, mm;
            t = count2 % 10;
            s = (count2 % 100) / 10;
            ss = (count2 % 600) / 100;
            m = (count2 / 600) % 10;
            mm = (count2 / 6000);
            DISPLAY(t, 100, s, ss, m, mm);
        } else if (mode == 6) {
            hex3_hex0_mode1_3_6(alarm_time);
            hex5_hex4_mode1_3_6(alarm_time);
            int s, ss, m, mm, h, hh;
            s = alarm_time % 10;
            ss = (alarm_time % 60) / 10;
            m = (alarm_time % 600) / 60;
            mm = (alarm_time % 3600) / 600;
            h = (alarm_time / 3600) % 10;
            hh = (alarm_time / 36000);
            DISPLAY(s, ss, m, mm, h, hh);
        }
        while (mode == 7) {
            *(KEY_ptr + 2) = 14;
            game();
        }
    }
}