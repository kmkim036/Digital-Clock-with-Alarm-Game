#include <stdlib.h>
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

#define PIXEL(r, g, b) (short int)((((r)&0x1F) << 11) | (((g)&0x3F) << 5) | (((b)&0x1F)))
#define FILL_PIXEL(x, y, r, g, b) *(short int*)(pixel_buffer_start + (((y)&0xFF) << 10) + (((x)&0x1FF) << 1)) = PIXEL(r, g, b)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

volatile int pixel_buffer_start; // global variable
volatile int* pixel_ctrl_ptr;
short int front_buffer[512 * 256];
short int back_buffer[512 * 256];

extern int mode;
extern int submode;

extern int laptime[5];
extern int lapcount;
extern int sw_c;

extern int alarm;

extern int ufo_x;
extern int dx;
extern int dy;
extern int bullet_x;
extern int bullet_y;
extern int hp;
extern int man_x;

void swap(int*, int*);
void wait_for_vsync();
void draw_pixel(int, int, short int);
void draw_line(int, int, int, int, short int);
void draw_square(int x1, int y1, int x2, int y2, int r, int g, int b);
void clear_screen();

void zero(int x);
void one(int x);
void two(int x);
void three(int x);
void four(int x);
void five(int x);
void six(int x);
void seven(int x);
void eight(int x);
void nine(int x);

void dot();

void zero2(int x, int y);
void one2(int x, int y);
void two2(int x, int y);
void three2(int x, int y);
void four2(int x, int y);
void five2(int x, int y);
void six2(int x, int y);
void seven2(int x, int y);
void eight2(int x, int y);
void nine2(int x, int y);

void zero3(int x);
void one3(int x);
void two3(int x);
void three3(int x);
void four3(int x);
void five3(int x);
void six3(int x);
void seven3(int x);
void eight3(int x);
void nine3(int x);
void seoul();
void beijing();
void moscow();
void ny();
void la();
void rome();
void map();

void zero4(int x, int y);
void one4(int x, int y);
void two4(int x, int y);
void three4(int x, int y);
void four4(int x, int y);
void five4(int x, int y);
void six4(int x, int y);
void seven4(int x, int y);
void eight4(int x, int y);
void nine4(int x, int y);
void dot4(int y);
void DISPLAY4(int rrr, int rr, int r, int l, int ll, int lll, int y);

void heart(int x);
void game();

void modenumber();
void DISPLAY(int rrr, int rr, int r, int l, int ll, int lll);
