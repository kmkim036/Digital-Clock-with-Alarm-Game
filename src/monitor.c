#include <monitor.h>

//functions for drawing
void swap(int* a, int* b)
{
    int tmp = 0;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void wait_for_vsync()
{
    register int status;
    *pixel_ctrl_ptr = 1; // start the synchronization process
    status = *(pixel_ctrl_ptr + 3);
    while ((status & 0x01) != 0)
        status = *(pixel_ctrl_ptr + 3);
}

void draw_pixel(int x, int y, short int line_color)
{
    *(short int*)(pixel_buffer_start + ((y & 0xFF) << 10) + ((x & 0x1FF) << 1)) = line_color;
}

void draw_line(int x0, int y0, int x1, int y1, short int line_color)
{
    int is_steep = abs(y1 - y0) - abs(x1 - x0);
    int deltax, deltay, error;
    int x, y, y_step;

    if (is_steep > 0) {
        swap(&x0, &y0);
        swap(&x1, &y1);
    }

    if (x0 > x1) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    deltax = x1 - x0;
    deltay = abs(y1 - y0);
    error = -(deltax / 2);
    y = y0;

    if (y0 < y1)
        y_step = 1;
    else
        y_step = -1;

    for (x = x0; x < x1; x++) {
        if (is_steep > 0)
            draw_pixel(y, x, line_color);
        else
            draw_pixel(x, y, line_color);

        error = error + deltay;
        if (error >= 0) {
            y = y + y_step;
            error = error - deltax;
        }
    }
}

void draw_square(int x1, int y1, int x2, int y2, int r, int g, int b)
{
    int x, y;
    for (x = x1; x <= x2; x++) {
        for (y = y1; y <= y2; y++) {
            FILL_PIXEL(x, y, r, g, b);
        }
    }
}

void clear_screen()
{
    int x, y;
    for (x = 0; x < SCREEN_WIDTH; x++) {
        for (y = 0; y < SCREEN_HEIGHT; y++) {
            draw_pixel(x, y, 0);
        }
    }
}

//draw zero to nine for mode 1, 5, 6
void zero(int x)
{
    draw_line(x - 10, 100, x + 10, 100, 0xFF); //0
    draw_line(x + 10, 100, x + 10, 140, 0xFF); //0
    draw_line(x - 10, 100, x - 10, 140, 0xFF); //0
    draw_line(x - 10, 140, x + 10, 140, 0xFF); //0
}

void one(int x)
{
    draw_line(x + 10, 100, x + 10, 140, 0xFF); //1
}

void two(int x)
{
    draw_line(x - 10, 100, x + 10, 100, 0xFF); //2
    draw_line(x + 10, 100, x + 10, 120, 0xFF); //2
    draw_line(x - 10, 120, x + 10, 120, 0xFF); //2
    draw_line(x - 10, 120, x - 10, 140, 0xFF); //2
    draw_line(x - 10, 140, x + 10, 140, 0xFF); //2
}

void three(int x)
{
    draw_line(x - 10, 100, x + 10, 100, 0xFF); //3
    draw_line(x + 10, 100, x + 10, 140, 0xFF); //3
    draw_line(x - 10, 120, x + 10, 120, 0xFF); //3
    draw_line(x - 10, 140, x + 10, 140, 0xFF); //3
}

void four(int x)
{
    draw_line(x - 10, 100, x - 10, 120, 0xFF); //4
    draw_line(x + 10, 100, x + 10, 140, 0xFF); //4
    draw_line(x - 10, 120, x + 10, 120, 0xFF); //4
}

void five(int x)
{
    draw_line(x - 10, 100, x + 10, 100, 0xFF); //5
    draw_line(x - 10, 100, x - 10, 120, 0xFF); //5
    draw_line(x - 10, 120, x + 10, 120, 0xFF); //5
    draw_line(x + 10, 120, x + 10, 140, 0xFF); //5
    draw_line(x - 10, 140, x + 10, 140, 0xFF); //5
}

void six(int x)
{
    draw_line(x - 10, 100, x + 10, 100, 0xFF); //6
    draw_line(x - 10, 120, x + 10, 120, 0xFF); //6
    draw_line(x - 10, 140, x + 10, 140, 0xFF); //6
    draw_line(x + 10, 120, x + 10, 140, 0xFF); //6
    draw_line(x - 10, 100, x - 10, 140, 0xFF); //6
}

void seven(int x)
{
    draw_line(x - 10, 100, x + 10, 100, 0xFF); //7
    draw_line(x + 10, 100, x + 10, 140, 0xFF); //7
}

void eight(int x)
{
    draw_line(x - 10, 100, x + 10, 100, 0xFF); //8
    draw_line(x - 10, 120, x + 10, 120, 0xFF); //8
    draw_line(x - 10, 140, x + 10, 140, 0xFF); //8
    draw_line(x + 10, 100, x + 10, 140, 0xFF); //8
    draw_line(x - 10, 100, x - 10, 140, 0xFF); //8
}

void nine(int x)
{
    draw_line(x - 10, 100, x + 10, 100, 0xFF); //9
    draw_line(x - 10, 120, x + 10, 120, 0xFF); //9
    draw_line(x - 10, 140, x + 10, 140, 0xFF); //9
    draw_line(x + 10, 100, x + 10, 140, 0xFF); //9
    draw_line(x - 10, 100, x - 10, 120, 0xFF); //9
}

//draw dot for mode 5
void dot()
{
    FILL_PIXEL(220, 130, 0xFF, 0xFF, 0xFF);
}

//draw zero to nine for mode 2
void zero2(int x, int y)
{
    draw_line(x - 10, 0 + y, x + 10, +y, 0xFF); //0
    draw_line(x + 10, 0 + y, x + 10, 40 + y, 0xFF); //0
    draw_line(x - 10, 0 + y, x - 10, 40 + y, 0xFF); //0
    draw_line(x - 10, 40 + y, x + 10, 40 + y, 0xFF); //0
}

void one2(int x, int y)
{
    draw_line(x + 10, y, x + 10, 40 + y, 0xFF); //1
}

void two2(int x, int y)
{
    draw_line(x - 10, y, x + 10, y, 0xFF); //2
    draw_line(x + 10, y, x + 10, 20 + y, 0xFF); //2
    draw_line(x - 10, 20 + y, x + 10, 20 + y, 0xFF); //2
    draw_line(x - 10, 20 + y, x - 10, 40 + y, 0xFF); //2
    draw_line(x - 10, 40 + y, x + 10, 40 + y, 0xFF); //2
}

void three2(int x, int y)
{
    draw_line(x - 10, +y, x + 10, +y, 0xFF); //3
    draw_line(x + 10, +y, x + 10, 40 + y, 0xFF); //3
    draw_line(x - 10, 20 + y, x + 10, 20 + y, 0xFF); //3
    draw_line(x - 10, 40 + y, x + 10, 40 + y, 0xFF); //3
}

void four2(int x, int y)
{
    draw_line(x - 10, +y, x - 10, 20 + y, 0xFF); //4
    draw_line(x + 10, +y, x + 10, 40 + y, 0xFF); //4
    draw_line(x - 10, 20 + y, x + 10, 20 + y, 0xFF); //4
}

void five2(int x, int y)
{
    draw_line(x - 10, +y, x + 10, +y, 0xFF); //5
    draw_line(x - 10, +y, x - 10, 20 + y, 0xFF); //5
    draw_line(x - 10, 20 + y, x + 10, 20 + y, 0xFF); //5
    draw_line(x + 10, 20 + y, x + 10, 40 + y, 0xFF); //5
    draw_line(x - 10, 40 + y, x + 10, 40 + y, 0xFF); //5
}

void six2(int x, int y)
{
    draw_line(x - 10, +y, x + 10, +y, 0xFF); //6
    draw_line(x - 10, 20 + y, x + 10, 20 + y, 0xFF); //6
    draw_line(x - 10, 40 + y, x + 10, 40 + y, 0xFF); //6
    draw_line(x + 10, 20 + y, x + 10, 40 + y, 0xFF); //6
    draw_line(x - 10, +y, x - 10, 40 + y, 0xFF); //6
}

void seven2(int x, int y)
{
    draw_line(x - 10, +y, x + 10, +y, 0xFF); //7
    draw_line(x + 10, +y, x + 10, 40 + y, 0xFF); //7
}

void eight2(int x, int y)
{
    draw_line(x - 10, +y, x + 10, +y, 0xFF); //8
    draw_line(x - 10, 20 + y, x + 10, 20 + y, 0xFF); //8
    draw_line(x - 10, 40 + y, x + 10, 40 + y, 0xFF); //8
    draw_line(x + 10, +y, x + 10, 40 + y, 0xFF); //8
    draw_line(x - 10, +y, x - 10, 40 + y, 0xFF); //8
}

void nine2(int x, int y)
{
    draw_line(x - 10, +y, x + 10, +y, 0xFF); //9
    draw_line(x - 10, 20 + y, x + 10, 20 + y, 0xFF); //9
    draw_line(x - 10, 40 + y, x + 10, 40 + y, 0xFF); //9
    draw_line(x + 10, +y, x + 10, 40 + y, 0xFF); //9
    draw_line(x - 10, +y, x - 10, 20 + y, 0xFF); //9
}

//draw zero to nine for mode 3
void zero3(int x)
{
    draw_line(x - 5, 180, x + 5, 180, 0xFF); //0
    draw_line(x + 5, 180, x + 5, 220, 0xFF); //0
    draw_line(x - 5, 180, x - 5, 220, 0xFF); //0
    draw_line(x - 5, 220, x + 5, 220, 0xFF); //0
}

void one3(int x)
{
    draw_line(x + 5, 180, x + 5, 220, 0xFF); //1
}

void two3(int x)
{
    draw_line(x - 5, 180, x + 5, 180, 0xFF); //2
    draw_line(x + 5, 180, x + 5, 200, 0xFF); //2
    draw_line(x - 5, 200, x + 5, 200, 0xFF); //2
    draw_line(x - 5, 200, x - 5, 220, 0xFF); //2
    draw_line(x - 5, 220, x + 5, 220, 0xFF); //2
}

void three3(int x)
{
    draw_line(x - 5, 180, x + 5, 180, 0xFF); //3
    draw_line(x + 5, 180, x + 5, 220, 0xFF); //3
    draw_line(x - 5, 200, x + 5, 200, 0xFF); //3
    draw_line(x - 5, 220, x + 5, 220, 0xFF); //3
}

void four3(int x)
{
    draw_line(x - 5, 180, x - 5, 200, 0xFF); //4
    draw_line(x + 5, 180, x + 5, 220, 0xFF); //4
    draw_line(x - 5, 200, x + 5, 200, 0xFF); //4
}

void five3(int x)
{
    draw_line(x - 5, 180, x + 5, 180, 0xFF); //5
    draw_line(x - 5, 180, x - 5, 200, 0xFF); //5
    draw_line(x - 5, 200, x + 5, 200, 0xFF); //5
    draw_line(x + 5, 200, x + 5, 220, 0xFF); //5
    draw_line(x - 5, 220, x + 5, 220, 0xFF); //5
}

void six3(int x)
{
    draw_line(x - 5, 180, x + 5, 180, 0xFF); //6
    draw_line(x - 5, 200, x + 5, 200, 0xFF); //6
    draw_line(x - 5, 220, x + 5, 220, 0xFF); //6
    draw_line(x + 5, 200, x + 5, 220, 0xFF); //6
    draw_line(x - 5, 180, x - 5, 220, 0xFF); //6
}

void seven3(int x)
{
    draw_line(x - 5, 180, x + 5, 180, 0xFF); //7
    draw_line(x + 5, 180, x + 5, 220, 0xFF); //7
}

void eight3(int x)
{
    draw_line(x - 5, 180, x + 5, 180, 0xFF); //8
    draw_line(x - 5, 200, x + 5, 200, 0xFF); //8
    draw_line(x - 5, 220, x + 5, 220, 0xFF); //8
    draw_line(x + 5, 180, x + 5, 220, 0xFF); //8
    draw_line(x - 5, 180, x - 5, 220, 0xFF); //8
}

void nine3(int x)
{
    draw_line(x - 5, 180, x + 5, 180, 0xFF); //9
    draw_line(x - 5, 200, x + 5, 200, 0xFF); //9
    draw_line(x - 5, 220, x + 5, 220, 0xFF); //9
    draw_line(x + 5, 180, x + 5, 220, 0xFF); //9
    draw_line(x - 5, 180, x - 5, 200, 0xFF); //9
}

//name of city for mode 3
void seoul()
{
    draw_line(30 - 20, 180, 50 - 20, 180, 0xFF);
    draw_line(30 - 20, 180, 30 - 20, 200, 0xFF);
    draw_line(30 - 20, 200, 50 - 20, 200, 0xFF);
    draw_line(50 - 20, 200, 50 - 20, 220, 0xFF);
    draw_line(30 - 20, 220, 50 - 20, 220, 0xFF);

    draw_line(30 + 21 - 17, 180, 50 + 21 - 17, 180, 0xFF);
    draw_line(30 + 21 - 17, 180, 30 + 21 - 17, 220, 0xFF);
    draw_line(30 + 21 - 17, 200, 50 + 21 - 17, 200, 0xFF);
    draw_line(30 + 21 - 17, 220, 50 + 21 - 17, 220, 0xFF);

    draw_line(30 + 42 - 14, 180, 50 + 42 - 14, 180, 0xFF);
    draw_line(30 + 42 - 14, 180, 30 + 42 - 14, 220, 0xFF);
    draw_line(30 + 42 - 14, 220, 50 + 42 - 14, 220, 0xFF);
    draw_line(50 + 42 - 14, 180, 50 + 42 - 14, 220, 0xFF);

    draw_line(30 + 63 - 11, 180, 30 + 63 - 11, 220, 0xFF);
    draw_line(30 + 63 - 11, 220, 50 + 63 - 11, 220, 0xFF);
    draw_line(50 + 63 - 11, 180, 50 + 63 - 11, 220, 0xFF);

    draw_line(30 + 84 - 8, 180, 30 + 84 - 8, 220, 0xFF);
    draw_line(30 + 84 - 8, 220, 50 + 84 - 8, 220, 0xFF);
}

void beijing()
{
    draw_line(30 - 20, 180, 40 - 20, 180, 0xFF);
    draw_line(40 - 20, 180, 50 - 20, 190, 0xFF);
    draw_line(50 - 20, 190, 40 - 20, 200, 0xFF);
    draw_line(30 - 20, 200, 40 - 20, 200, 0xFF);
    draw_line(40 - 20, 200, 50 - 20, 210, 0xFF);
    draw_line(50 - 20, 210, 40 - 20, 220, 0xFF);
    draw_line(40 - 20, 220, 30 - 20, 220, 0xFF);
    draw_line(30 - 20, 180, 30 - 20, 220, 0xFF);

    draw_line(30 + 21 - 17, 180, 50 + 21 - 17, 180, 0xFF);
    draw_line(30 + 21 - 17, 180, 30 + 21 - 17, 220, 0xFF);
    draw_line(30 + 21 - 17, 200, 50 + 21 - 17, 200, 0xFF);
    draw_line(30 + 21 - 17, 220, 50 + 21 - 17, 220, 0xFF);

    draw_line(40 + 42 - 14, 180, 40 + 42 - 14, 220, 0xFF);

    draw_line(30 + 63 - 11, 180, 50 + 63 - 11, 180, 0xFF);
    draw_line(40 + 63 - 11, 180, 40 + 63 - 11, 220, 0xFF);
    draw_line(40 + 63 - 11, 220, 35 + 63 - 11, 200, 0xFF);

    draw_line(40 + 84 - 8, 180, 40 + 84 - 8, 220, 0xFF);

    draw_line(30 + 105 - 5, 180, 30 + 105 - 5, 220, 0xFF);
    draw_line(50 + 105 - 5, 180, 50 + 105 - 5, 220, 0xFF);
    draw_line(30 + 105 - 5, 180, 50 + 105 - 5, 220, 0xFF);

    draw_line(30 + 126 - 2, 180, 50 + 126 - 2, 180, 0xFF);
    draw_line(30 + 126 - 2, 180, 30 + 126 - 2, 220, 0xFF);
    draw_line(50 + 126 - 2, 180, 50 + 126 - 2, 190, 0xFF);
    draw_line(50 + 126 - 2, 210, 50 + 126 - 2, 220, 0xFF);
    draw_line(45 + 126 - 2, 210, 50 + 126 - 2, 210, 0xFF);
    draw_line(30 + 126 - 2, 220, 50 + 126 - 2, 220, 0xFF);
}

void moscow()
{
    draw_line(30 - 20, 180, 30 - 20, 220, 0xFF);
    draw_line(30 - 20, 180, 40 - 20, 220, 0xFF);
    draw_line(40 - 20, 220, 50 - 20, 180, 0xFF);
    draw_line(50 - 20, 180, 50 - 20, 220, 0xFF);

    draw_line(30 + 23 - 17, 180, 50 + 21 - 17, 180, 0xFF);
    draw_line(30 + 23 - 17, 180, 30 + 23 - 17, 220, 0xFF);
    draw_line(30 + 23 - 17, 220, 50 + 21 - 17, 220, 0xFF);
    draw_line(50 + 21 - 17, 180, 50 + 21 - 17, 220, 0xFF);

    draw_line(30 + 44 - 14, 180, 50 + 44 - 14, 180, 0xFF);
    draw_line(30 + 44 - 14, 180, 30 + 44 - 14, 200, 0xFF);
    draw_line(30 + 44 - 14, 200, 50 + 44 - 14, 200, 0xFF);
    draw_line(50 + 44 - 14, 200, 50 + 44 - 14, 220, 0xFF);
    draw_line(30 + 44 - 14, 220, 50 + 44 - 14, 220, 0xFF);

    draw_line(30 + 63 - 11, 180, 50 + 63 - 11, 180, 0xFF);
    draw_line(30 + 63 - 11, 180, 30 + 63 - 11, 220, 0xFF);
    draw_line(30 + 63 - 11, 220, 50 + 63 - 11, 220, 0xFF);

    draw_line(30 + 84 - 8, 180, 50 + 84 - 8, 180, 0xFF);
    draw_line(30 + 84 - 8, 180, 30 + 84 - 8, 220, 0xFF);
    draw_line(50 + 84 - 8, 180, 50 + 84 - 8, 220, 0xFF);
    draw_line(30 + 84 - 8, 220, 50 + 84 - 8, 220, 0xFF);

    draw_line(30 + 105 - 5, 180, 35 + 105 - 5, 220, 0xFF);
    draw_line(35 + 105 - 5, 220, 40 + 105 - 5, 180, 0xFF);
    draw_line(40 + 105 - 5, 180, 45 + 105 - 5, 220, 0xFF);
    draw_line(45 + 105 - 5, 220, 50 + 105 - 5, 180, 0xFF);
}

void ny()
{
    draw_line(30 - 20, 180, 30 - 20, 220, 0xFF);
    draw_line(50 - 20, 180, 50 - 20, 220, 0xFF);
    draw_line(30 - 20, 180, 50 - 20, 220, 0xFF);

    draw_line(30 + 21 - 17, 180, 40 + 21 - 17, 190, 0xFF);
    draw_line(50 + 21 - 17, 180, 40 + 21 - 17, 190, 0xFF);
    draw_line(40 + 21 - 17, 190, 40 + 21 - 17, 220, 0xFF);
}

void la()
{
    draw_line(30 - 20, 180, 30 - 20, 220, 0xFF);
    draw_line(30 - 20, 220, 50 - 20, 220, 0xFF);

    draw_line(30 + 21 - 17, 220, 40 + 21 - 17, 180, 0xFF);
    draw_line(40 + 21 - 17, 180, 50 + 21 - 17, 220, 0xFF);
    draw_line(35 + 21 - 17, 200, 45 + 21 - 17, 200, 0xFF);
}

void rome()
{
    draw_line(30 - 20, 180, 40 - 20, 180, 0xFF);
    draw_line(40 - 20, 180, 50 - 20, 190, 0xFF);
    draw_line(50 - 20, 190, 40 - 20, 200, 0xFF);
    draw_line(30 - 20, 200, 40 - 20, 200, 0xFF);
    draw_line(30 - 20, 180, 30 - 20, 220, 0xFF);
    draw_line(40 - 20, 200, 50 - 20, 220, 0xFF);

    draw_line(30 + 21 - 17, 180, 50 + 21 - 17, 180, 0xFF);
    draw_line(30 + 21 - 17, 180, 30 + 21 - 17, 220, 0xFF);
    draw_line(30 + 21 - 17, 220, 50 + 21 - 17, 220, 0xFF);
    draw_line(50 + 21 - 17, 180, 50 + 21 - 17, 220, 0xFF);

    draw_line(30 + 42 - 14, 180, 30 + 42 - 14, 220, 0xFF);
    draw_line(30 + 42 - 14, 180, 40 + 42 - 14, 220, 0xFF);
    draw_line(40 + 42 - 14, 220, 50 + 42 - 14, 180, 0xFF);
    draw_line(50 + 42 - 14, 180, 50 + 42 - 14, 220, 0xFF);

    draw_line(30 + 63 - 11, 180, 50 + 63 - 11, 180, 0xFF);
    draw_line(30 + 63 - 11, 200, 50 + 63 - 11, 200, 0xFF);
    draw_line(30 + 63 - 11, 220, 50 + 63 - 11, 220, 0xFF);
    draw_line(30 + 63 - 11, 180, 30 + 63 - 11, 220, 0xFF);
}

//draw map for mode 3
void map(void)
{
    draw_square(40, 27, 50, 30, 0xFF, 0xFF, 0xFF);
    draw_square(20, 30, 75 - 5, 40, 0xFF, 0xFF, 0xFF);
    draw_square(70, 30, 77, 35, 0xFF, 0xFF, 0xFF);
    draw_square(30, 40, 70 - 5, 45, 0xFF, 0xFF, 0xFF);
    draw_square(40, 30, 75 - 5, 45, 0xFF, 0xFF, 0xFF);
    draw_square(30, 45, 80 - 5, 50, 0xFF, 0xFF, 0xFF);
    draw_square(45, 30, 85 - 5, 50, 0xFF, 0xFF, 0xFF);
    draw_square(35, 50, 90 - 5, 55, 0xFF, 0xFF, 0xFF);
    draw_square(80, 50, 98 - 5, 55, 0xFF, 0xFF, 0xFF);
    draw_square(93, 50, 100, 55, 0xFF, 0xFF, 0xFF);
    draw_square(98, 45, 105, 50, 0xFF, 0xFF, 0xFF);
    draw_square(98, 40, 105, 45, 0xFF, 0xFF, 0xFF);

    draw_square(35, 55, 100 - 5, 60, 0xFF, 0xFF, 0xFF);
    draw_square(40, 60, 95 - 5, 65, 0xFF, 0xFF, 0xFF);
    draw_square(42, 65, 93 - 5, 70, 0xFF, 0xFF, 0xFF);
    draw_square(45, 70, 59 - 5, 75, 0xFF, 0xFF, 0xFF);
    draw_square(88, 70, 90 - 5, 75, 0xFF, 0xFF, 0xFF);
    draw_square(50, 75, 64, 80, 0xFF, 0xFF, 0xFF);
    draw_square(50, 80, 64, 85, 0xFF, 0xFF, 0xFF);
    draw_square(55, 85, 69, 90, 0xFF, 0xFF, 0xFF);
    draw_square(58, 90, 71, 95, 0xFF, 0xFF, 0xFF);
    draw_square(71, 100, 91, 105, 0xFF, 0xFF, 0xFF);
    draw_square(85 - 19, 105, 115 - 19, 110, 0xFF, 0xFF, 0xFF);
    draw_square(85 - 19, 110, 125 - 19, 115, 0xFF, 0xFF, 0xFF);
    draw_square(85 - 19, 115, 125 - 19, 120, 0xFF, 0xFF, 0xFF);
    draw_square(90 - 19, 120, 120 - 19, 125, 0xFF, 0xFF, 0xFF);
    draw_square(95 - 19, 125, 120 - 19, 130, 0xFF, 0xFF, 0xFF);
    draw_square(95 - 19, 130, 115 - 19, 135, 0xFF, 0xFF, 0xFF);
    draw_square(95 - 19, 135, 110 - 19, 140, 0xFF, 0xFF, 0xFF);
    draw_square(90 - 19, 140, 105 - 19, 145, 0xFF, 0xFF, 0xFF);
    draw_square(90 - 19, 145, 100 - 19, 150, 0xFF, 0xFF, 0xFF);
    draw_square(90 - 19, 150, 100 - 19, 155, 0xFF, 0xFF, 0xFF);
    draw_square(85 - 19, 155, 95 - 19, 160, 0xFF, 0xFF, 0xFF);
    draw_square(85 - 19, 160, 90 - 19, 165, 0xFF, 0xFF, 0xFF);

    draw_square(190, 15, 210, 20, 0xFF, 0xFF, 0xFF);
    draw_square(160, 20, 220, 25, 0xFF, 0xFF, 0xFF);
    draw_square(150, 25, 160, 30, 0xFF, 0xFF, 0xFF);
    draw_square(155, 25, 260, 30, 0xFF, 0xFF, 0xFF);
    draw_square(145, 30, 165, 35, 0xFF, 0xFF, 0xFF);
    draw_square(150, 30, 280, 35, 0xFF, 0xFF, 0xFF);
    draw_square(145, 35, 155, 40, 0xFF, 0xFF, 0xFF);
    draw_square(145, 35, 285, 40, 0xFF, 0xFF, 0xFF);
    draw_square(155, 40, 255, 45, 0xFF, 0xFF, 0xFF);
    draw_square(260, 40, 265, 45, 0xFF, 0xFF, 0xFF);
    draw_square(140, 45, 250, 50, 0xFF, 0xFF, 0xFF);
    draw_square(260, 45, 265, 50, 0xFF, 0xFF, 0xFF);
    draw_square(130, 50, 245, 55, 0xFF, 0xFF, 0xFF);
    draw_square(255, 50, 260, 55, 0xFF, 0xFF, 0xFF);
    draw_square(130, 55, 140, 65, 0xFF, 0xFF, 0xFF);
    draw_square(150, 55, 155, 65, 0xFF, 0xFF, 0xFF);
    draw_square(165, 55, 170, 65, 0xFF, 0xFF, 0xFF);
    draw_square(170, 65, 175, 73, 0xFF, 0xFF, 0xFF);
    draw_square(180, 55, 240, 65, 0xFF, 0xFF, 0xFF);
    draw_square(155, 65, 160, 70, 0xFF, 0xFF, 0xFF);
    draw_square(170, 65, 235, 70, 0xFF, 0xFF, 0xFF);
    draw_square(170, 70, 180, 75, 0xFF, 0xFF, 0xFF);
    draw_square(185, 70, 235, 75, 0xFF, 0xFF, 0xFF);
    draw_square(175, 75, 183, 80, 0xFF, 0xFF, 0xFF);
    draw_square(178, 80, 186, 85, 0xFF, 0xFF, 0xFF);
    draw_square(190, 75, 230, 80, 0xFF, 0xFF, 0xFF);
    draw_square(190, 80, 200, 85, 0xFF, 0xFF, 0xFF);
    draw_square(215, 80, 225, 85, 0xFF, 0xFF, 0xFF);
    draw_square(192, 85, 200, 90, 0xFF, 0xFF, 0xFF);
    draw_square(195, 90, 200, 95, 0xFF, 0xFF, 0xFF);
    draw_square(198, 95, 200, 100, 0xFF, 0xFF, 0xFF);
    draw_square(200, 85, 202, 90, 0xFF, 0xFF, 0xFF);
    draw_square(202, 80, 205, 85, 0xFF, 0xFF, 0xFF);
    draw_square(220, 85, 225, 90, 0xFF, 0xFF, 0xFF);
    draw_square(250 - 20, 90, 260 - 20, 95, 0xFF, 0xFF, 0xFF);
    draw_square(255 - 20, 80, 260 - 20, 85, 0xFF, 0xFF, 0xFF);
    draw_square(240, 65, 245, 73, 0xFF, 0xFF, 0xFF);

    draw_square(270 - 30, 105 + 15, 275 - 30, 110 + 15, 0xFF, 0xFF, 0xFF);
    draw_square(280 - 30, 105 + 15, 285 - 30, 110 + 15, 0xFF, 0xFF, 0xFF);
    draw_square(265 - 30, 110 + 15, 285 - 30, 115 + 15, 0xFF, 0xFF, 0xFF);
    draw_square(265 - 30, 115 + 15, 285 - 30, 120 + 15, 0xFF, 0xFF, 0xFF);
    draw_square(260 - 30, 120 + 15, 290 - 30, 130 + 15, 0xFF, 0xFF, 0xFF);
    draw_square(275 - 30, 130 + 15, 285 - 30, 135 + 15, 0xFF, 0xFF, 0xFF);
    draw_square(140, 70, 150, 75, 0xFF, 0xFF, 0xFF);
    draw_square(135, 75, 150, 80, 0xFF, 0xFF, 0xFF);
    draw_square(165, 75, 170, 80, 0xFF, 0xFF, 0xFF);
    draw_square(130, 80, 165, 90, 0xFF, 0xFF, 0xFF);
    draw_square(130, 90, 175, 100, 0xFF, 0xFF, 0xFF);
    draw_square(135, 100, 170, 105, 0xFF, 0xFF, 0xFF);
    draw_square(145, 105, 170, 110, 0xFF, 0xFF, 0xFF);
    draw_square(150, 110, 170, 115, 0xFF, 0xFF, 0xFF);
    draw_square(150, 115, 165, 120, 0xFF, 0xFF, 0xFF);
    draw_square(150, 120, 165, 130, 0xFF, 0xFF, 0xFF);
    draw_square(155, 130, 165, 140, 0xFF, 0xFF, 0xFF);
    draw_square(160, 140, 165, 145, 0xFF, 0xFF, 0xFF);
    draw_square(162, 145, 165, 150, 0xFF, 0xFF, 0xFF);
}

//draw zero to nine for mode 4
void zero4(int x, int y)
{
    draw_line(x - 5, 0 + y, x + 5, +y, 0xFF); //0
    draw_line(x + 5, 0 + y, x + 5, 20 + y, 0xFF); //0
    draw_line(x - 5, 0 + y, x - 5, 20 + y, 0xFF); //0
    draw_line(x - 5, 20 + y, x + 5, 20 + y, 0xFF); //0
}

void one4(int x, int y)
{
    draw_line(x + 5, y, x + 5, 20 + y, 0xFF); //1
}

void two4(int x, int y)
{
    draw_line(x - 5, y, x + 5, y, 0xFF); //2
    draw_line(x + 5, y, x + 5, 10 + y, 0xFF); //2
    draw_line(x - 5, 10 + y, x + 5, 10 + y, 0xFF); //2
    draw_line(x - 5, 10 + y, x - 5, 20 + y, 0xFF); //2
    draw_line(x - 5, 20 + y, x + 5, 20 + y, 0xFF); //2
}

void three4(int x, int y)
{
    draw_line(x - 5, +y, x + 5, +y, 0xFF); //3
    draw_line(x + 5, +y, x + 5, 20 + y, 0xFF); //3
    draw_line(x - 5, 10 + y, x + 5, 10 + y, 0xFF); //3
    draw_line(x - 5, 20 + y, x + 5, 20 + y, 0xFF); //3
}

void four4(int x, int y)
{
    draw_line(x - 5, +y, x - 5, 10 + y, 0xFF); //4
    draw_line(x + 5, +y, x + 5, 20 + y, 0xFF); //4
    draw_line(x - 5, 10 + y, x + 5, 10 + y, 0xFF); //4
}

void five4(int x, int y)
{
    draw_line(x - 5, +y, x + 5, +y, 0xFF); //5
    draw_line(x - 5, +y, x - 5, 10 + y, 0xFF); //5
    draw_line(x - 5, 10 + y, x + 5, 10 + y, 0xFF); //5
    draw_line(x + 5, 10 + y, x + 5, 20 + y, 0xFF); //5
    draw_line(x - 5, 20 + y, x + 5, 20 + y, 0xFF); //5
}

void six4(int x, int y)
{
    draw_line(x - 5, +y, x + 5, +y, 0xFF); //6
    draw_line(x - 5, 10 + y, x + 5, 10 + y, 0xFF); //6
    draw_line(x - 5, 20 + y, x + 5, 20 + y, 0xFF); //6
    draw_line(x + 5, 10 + y, x + 5, 20 + y, 0xFF); //6
    draw_line(x - 5, +y, x - 5, 20 + y, 0xFF); //6
}

void seven4(int x, int y)
{
    draw_line(x - 5, +y, x + 5, +y, 0xFF); //7
    draw_line(x + 5, +y, x + 5, 20 + y, 0xFF); //7
}

void eight4(int x, int y)
{
    draw_line(x - 5, +y, x + 5, +y, 0xFF); //8
    draw_line(x - 5, 10 + y, x + 5, 10 + y, 0xFF); //8
    draw_line(x - 5, 20 + y, x + 5, 20 + y, 0xFF); //8
    draw_line(x + 5, +y, x + 5, 20 + y, 0xFF); //8
    draw_line(x - 5, +y, x - 5, 20 + y, 0xFF); //8
}

void nine4(int x, int y)
{
    draw_line(x - 5, +y, x + 5, +y, 0xFF); //9
    draw_line(x - 5, 10 + y, x + 5, 10 + y, 0xFF); //9
    draw_line(x - 5, 20 + y, x + 5, 20 + y, 0xFF); //9
    draw_line(x + 5, +y, x + 5, 20 + y, 0xFF); //9
    draw_line(x - 5, +y, x - 5, 10 + y, 0xFF); //9
}

//draw dot for mode 4
void dot4(int y)
{
    FILL_PIXEL(220, y + 20, 0xFF, 0xFF, 0xFF);
}

//for mode 4
void DISPLAY4(int rrr, int rr, int r, int l, int ll, int lll, int y)
{
    switch (rrr) {
    case 1:
        one4(260, y);
        break;
    case 2:
        two4(260, y);
        break;
    case 3:
        three4(260, y);
        break;
    case 4:
        four4(260, y);
        break;
    case 5:
        five4(260, y);
        break;
    case 6:
        six4(260, y);
        break;
    case 7:
        seven4(260, y);
        break;
    case 8:
        eight4(260, y);
        break;
    case 9:
        nine4(260, y);
        break;
    case 0:
        zero4(260, y);
        break;
    default:
        one4(260, y);
        break;
    }
    switch (rr) {
    case 1:
        one4(220, y);
        break;
    case 2:
        two4(220, y);
        break;
    case 3:
        three4(220, y);
        break;
    case 4:
        four4(220, y);
        break;
    case 5:
        five4(220, y);
        break;
    case 6:
        six4(220, y);
        break;
    case 7:
        seven4(220, y);
        break;
    case 8:
        eight4(220, y);
        break;
    case 9:
        nine4(220, y);
        break;
    case 0:
        zero4(220, y);
        break;
    default:
        dot4(y);
        break;
    }
    switch (r) {
    case 1:
        one4(180, y);
        break;
    case 2:
        two4(180, y);
        break;
    case 3:
        three4(180, y);
        break;
    case 4:
        four4(180, y);
        break;
    case 5:
        five4(180, y);
        break;
    case 6:
        six4(180, y);
        break;
    case 7:
        seven4(180, y);
        break;
    case 8:
        eight4(180, y);
        break;
    case 9:
        nine4(180, y);
        break;
    case 0:
        zero4(180, y);
        break;
    default:
        one4(180, y);
        break;
    }
    switch (l) {
    case 1:
        one4(140, y);
        break;
    case 2:
        two4(140, y);
        break;
    case 3:
        three4(140, y);
        break;
    case 4:
        four4(140, y);
        break;
    case 5:
        five4(140, y);
        break;
    case 6:
        six4(140, y);
        break;
    case 7:
        seven4(140, y);
        break;
    case 8:
        eight4(140, y);
        break;
    case 9:
        nine4(140, y);
        break;
    case 0:
        zero4(140, y);
        break;
    default:
        one4(140, y);
        break;
    }
    switch (ll) {
    case 1:
        one4(100, y);
        break;
    case 2:
        two4(100, y);
        break;
    case 3:
        three4(100, y);
        break;
    case 4:
        four4(100, y);
        break;
    case 5:
        five4(100, y);
        break;
    case 6:
        six4(100, y);
        break;
    case 7:
        seven4(100, y);
        break;
    case 8:
        eight4(100, y);
        break;
    case 9:
        nine4(100, y);
        break;
    case 0:
        zero4(100, y);
        break;
    default:
        one4(100, y);
        break;
    }
    switch (lll) {
    case 1:
        one4(60, y);
        break;
    case 2:
        two4(60, y);
        break;
    case 3:
        three4(60, y);
        break;
    case 4:
        four4(60, y);
        break;
    case 5:
        five4(60, y);
        break;
    case 6:
        six4(60, y);
        break;
    case 7:
        seven4(60, y);
        break;
    case 8:
        eight4(60, y);
        break;
    case 9:
        nine4(60, y);
        break;
    case 0:
        zero4(60, y);
        break;
    default:
        one4(60, y);
        break;
    }
}

void heart(int x)
{
    draw_line(x - 3, 219, x - 3, 221, 0xF800);
    draw_line(x - 3, 219, x - 2, 217, 0xF800);
    draw_line(x - 2, 217, x - 1, 217, 0xF800);
    draw_line(x - 1, 217, x, 219, 0xF800);
    draw_line(x, 219, x + 1, 217, 0xF800);
    draw_line(x + 1, 217, x + 2, 217, 0xF800);
    draw_line(x + 3, 219, x + 2, 217, 0xF800);
    draw_line(x + 3, 219, x + 3, 221, 0xF800);
    draw_line(x - 3, 221, x, 223, 0xF800);
    draw_line(x + 3, 221, x, 223, 0xF800);
}

void game()
{
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); //to draw on the back buffer
    clear_screen(); //clear back buffer
    seven4(5, 5);

    //ufo
    draw_square(ufo_x, 10, ufo_x + 10, 12, 0xFF, 0, 0xFF);
    draw_square(ufo_x - 2, 12, ufo_x + 12, 14, 0xFF, 0, 0xFF);
    draw_square(ufo_x, 14, ufo_x + 10, 16, 0xFF, 0, 0xFF);
    ufo_x += dx;
    if ((ufo_x + dx) >= SCREEN_WIDTH - 12)
        dx = -dx;
    if ((ufo_x + dx) <= 0)
        dx = -dx;

    //man
    draw_square(man_x - 2, 230, man_x + 2, 235, 0xFF, 0xFF, 0);

    //bullet
    bullet_y += dy;
    FILL_PIXEL(bullet_x, bullet_y, 0xFF, 0xFF, 0xFF);
    if (bullet_y < 0)
        dy = 0;
    if (bullet_x > (ufo_x - 3) && bullet_x < (ufo_x + 13))
        if (bullet_y > 9 && bullet_y < 17) {
            bullet_y = SCREEN_HEIGHT;
            dy = 0;
            hp--;
        }

    for (int i = 1; i < hp + 1; i++)
        heart(10 * i);

    if (hp == 0)
        mode = 1;

    wait_for_vsync();
}

void modenumber()
{
    switch (mode) {
    case 1:
        one4(5, 5);
        break;
    case 2:
        two4(5, 5);
        break;
    case 3:
        three4(5, 5);
        break;
    case 4:
        four4(5, 5);
        break;
    case 5:
        five4(5, 5);
        break;
    case 6:
        six4(5, 5);
        break;
    default:
        break;
    }
}

void DISPLAY(int rrr, int rr, int r, int l, int ll, int lll)
{
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); //to draw on the back buffer
    clear_screen(); //clear back buffer
    modenumber();

    if (mode == 3) {
        map();
        switch (submode) {
        case 1:
            seoul();
            FILL_PIXEL(242, 70, 0xFF, 0x00, 0x00);
            break; //Seoul
        case 2:
            beijing();
            FILL_PIXEL(230, 68, 0xFF, 0x00, 0x00);
            break; //Beijing
        case 3:
            moscow();
            FILL_PIXEL(180, 40, 0xFF, 0x00, 0x00);
            break; //Moscow
        case 4:
            rome();
            FILL_PIXEL(152, 60, 0xFF, 0x00, 0x00);
            break; //London
        case 5:
            ny();
            FILL_PIXEL(90, 53, 0xFF, 0x00, 0x00);
            break; //NY
        case 6:
            la();
            FILL_PIXEL(47, 65, 0xFF, 0x00, 0x00);
            break; //LA
        }
        FILL_PIXEL(230, 190, 0xFF, 0xFF, 0xFF);
        FILL_PIXEL(230, 210, 0xFF, 0xFF, 0xFF);
        FILL_PIXEL(270, 190, 0xFF, 0xFF, 0xFF);
        FILL_PIXEL(270, 210, 0xFF, 0xFF, 0xFF);
        switch (rrr) {
        case 1:
            one3(300);
            break;
        case 2:
            two3(300);
            break;
        case 3:
            three3(300);
            break;
        case 4:
            four3(300);
            break;
        case 5:
            five3(300);
            break;
        case 6:
            six3(300);
            break;
        case 7:
            seven3(300);
            break;
        case 8:
            eight3(300);
            break;
        case 9:
            nine3(300);
            break;
        case 0:
            zero3(300);
            break;
        default:
            one3(300);
            break;
        }
        switch (rr) {
        case 1:
            one3(280);
            break;
        case 2:
            two3(280);
            break;
        case 3:
            three3(280);
            break;
        case 4:
            four3(280);
            break;
        case 5:
            five3(280);
            break;
        case 6:
            six3(280);
            break;
        case 7:
            seven3(280);
            break;
        case 8:
            eight3(280);
            break;
        case 9:
            nine3(280);
            break;
        case 0:
            zero3(280);
            break;
        default:
            dot();
            break;
        }
        switch (r) {
        case 1:
            one3(260);
            break;
        case 2:
            two3(260);
            break;
        case 3:
            three3(260);
            break;
        case 4:
            four3(260);
            break;
        case 5:
            five3(260);
            break;
        case 6:
            six3(260);
            break;
        case 7:
            seven3(260);
            break;
        case 8:
            eight3(260);
            break;
        case 9:
            nine3(260);
            break;
        case 0:
            zero3(260);
            break;
        default:
            one(260);
            break;
        }
        switch (l) {
        case 1:
            one3(240);
            break;
        case 2:
            two3(240);
            break;
        case 3:
            three3(240);
            break;
        case 4:
            four3(240);
            break;
        case 5:
            five3(240);
            break;
        case 6:
            six3(240);
            break;
        case 7:
            seven3(240);
            break;
        case 8:
            eight3(240);
            break;
        case 9:
            nine3(240);
            break;
        case 0:
            zero3(240);
            break;
        default:
            one3(240);
            break;
        }
        switch (ll) {
        case 1:
            one3(220);
            break;
        case 2:
            two3(220);
            break;
        case 3:
            three3(220);
            break;
        case 4:
            four3(220);
            break;
        case 5:
            five3(220);
            break;
        case 6:
            six3(220);
            break;
        case 7:
            seven3(220);
            break;
        case 8:
            eight3(220);
            break;
        case 9:
            nine3(220);
            break;
        case 0:
            zero3(220);
            break;
        default:
            one3(220);
            break;
        }
        switch (lll) {
        case 1:
            one3(200);
            break;
        case 2:
            two3(200);
            break;
        case 3:
            three3(200);
            break;
        case 4:
            four3(200);
            break;
        case 5:
            five3(200);
            break;
        case 6:
            six3(200);
            break;
        case 7:
            seven3(200);
            break;
        case 8:
            eight3(200);
            break;
        case 9:
            nine3(200);
            break;
        case 0:
            zero3(200);
            break;
        default:
            one3(200);
            break;
        }
    } else if (mode == 4) {
        //stopwatch
        if (sw_c == 0) {
            FILL_PIXEL(120, 110, 0xFF, 0xFF, 0xFF);
            FILL_PIXEL(120, 130, 0xFF, 0xFF, 0xFF);
            switch (rrr) {
            case 1:
                one(260);
                break;
            case 2:
                two(260);
                break;
            case 3:
                three(260);
                break;
            case 4:
                four(260);
                break;
            case 5:
                five(260);
                break;
            case 6:
                six(260);
                break;
            case 7:
                seven(260);
                break;
            case 8:
                eight(260);
                break;
            case 9:
                nine(260);
                break;
            case 0:
                zero(260);
                break;
            default:
                one(260);
                break;
            }
            switch (rr) {
            case 1:
                one(220);
                break;
            case 2:
                two(220);
                break;
            case 3:
                three(220);
                break;
            case 4:
                four(220);
                break;
            case 5:
                five(220);
                break;
            case 6:
                six(220);
                break;
            case 7:
                seven(220);
                break;
            case 8:
                eight(220);
                break;
            case 9:
                nine(220);
                break;
            case 0:
                zero(220);
                break;
            default:
                dot();
                break;
            }
            switch (r) {
            case 1:
                one(180);
                break;
            case 2:
                two(180);
                break;
            case 3:
                three(180);
                break;
            case 4:
                four(180);
                break;
            case 5:
                five(180);
                break;
            case 6:
                six(180);
                break;
            case 7:
                seven(180);
                break;
            case 8:
                eight(180);
                break;
            case 9:
                nine(180);
                break;
            case 0:
                zero(180);
                break;
            default:
                one(180);
                break;
            }
            switch (l) {
            case 1:
                one(140);
                break;
            case 2:
                two(140);
                break;
            case 3:
                three(140);
                break;
            case 4:
                four(140);
                break;
            case 5:
                five(140);
                break;
            case 6:
                six(140);
                break;
            case 7:
                seven(140);
                break;
            case 8:
                eight(140);
                break;
            case 9:
                nine(140);
                break;
            case 0:
                zero(140);
                break;
            default:
                one(140);
                break;
            }
            switch (ll) {
            case 1:
                one(100);
                break;
            case 2:
                two(100);
                break;
            case 3:
                three(100);
                break;
            case 4:
                four(100);
                break;
            case 5:
                five(100);
                break;
            case 6:
                six(100);
                break;
            case 7:
                seven(100);
                break;
            case 8:
                eight(100);
                break;
            case 9:
                nine(100);
                break;
            case 0:
                zero(100);
                break;
            default:
                one(100);
                break;
            }
            switch (lll) {
            case 1:
                one(60);
                break;
            case 2:
                two(60);
                break;
            case 3:
                three(60);
                break;
            case 4:
                four(60);
                break;
            case 5:
                five(60);
                break;
            case 6:
                six(60);
                break;
            case 7:
                seven(60);
                break;
            case 8:
                eight(60);
                break;
            case 9:
                nine(60);
                break;
            case 0:
                zero(60);
                break;
            default:
                one(60);
                break;
            }
        } else {
            int t[5], s[5], ss[5], m[5], mm[5], i;
            for (i = 0; i < 5; i++) {
                t[i] = laptime[i] % 10;
                s[i] = (laptime[i] % 100) / 10;
                ss[i] = (laptime[i] % 600) / 100;
                m[i] = (laptime[i] / 600) % 10;
                mm[i] = (laptime[i] / 6000);
            }
            DISPLAY4(rrr, rr, r, l, ll, lll, 10);
            FILL_PIXEL(120, 10 + 5, 0xFF, 0xFF, 0xFF);
            FILL_PIXEL(120, 10 + 10, 0xFF, 0xFF, 0xFF);
            draw_line(0, 40, SCREEN_WIDTH - 1, 40, 0xFF);
            for (i = 0; i < lapcount; i++) {
                DISPLAY4(t[i], 100, s[i], ss[i], m[i], mm[i], 50 + 30 * i);
                FILL_PIXEL(120, 50 + 30 * i + 5, 0xFF, 0xFF, 0xFF);
                FILL_PIXEL(120, 50 + 30 * i + 10, 0xFF, 0xFF, 0xFF);
            }
        }
    } else if (mode == 2) {
        draw_line(64 + 5, 104, 64 + 5, 122, 0xFF);
        draw_line(64 + 5, 122, 64 + 24, 122, 0xFF);

        draw_line(64 + 8, 127, 64 + 8, 138, 0xFF);
        draw_line(64 + 8, 138, 64 + 33, 138, 0xFF);

        draw_line(64 + 21, 108, 64 + 33, 108, 0xFF);
        draw_line(64 + 21, 115, 64 + 33, 115, 0xFF);
        draw_line(64 + 33, 102, 64 + 33, 124, 0xFF);

        draw_line(168 + 5, 108, 168 + 17, 102, 0xFF);
        draw_line(168 + 5, 108, 168 + 17, 110, 0xFF);
        draw_line(168 + 17, 102, 198, 108, 0xFF);
        draw_line(168 + 17, 110, 198, 108, 0xFF);
        draw_line(170, 116, 208, 116, 0xFF);
        draw_line(168 + 17, 116, 168 + 17, 124, 0xFF);
        draw_line(208, 100, 208, 123, 0xFF);
        draw_line(168 + 25, 120, 208, 120, 0xFF);
        draw_line(178, 126, 208, 126, 0xFF);
        draw_line(208, 126, 208, 134, 0xFF);
        draw_line(178, 134, 208, 134, 0xFF);
        draw_line(178, 134, 178, 140, 0xFF);
        draw_line(178, 140, 208, 140, 0xFF);

        draw_line(272 + 5, 108, 272 + 17, 102, 0xFF);
        draw_line(272 + 5, 108, 272 + 17, 115, 0xFF);
        draw_line(272 + 17, 102, 198 + 104, 108, 0xFF);
        draw_line(272 + 17, 115, 198 + 104, 108, 0xFF);

        draw_line(208 + 104, 100, 208 + 104, 123, 0xFF);

        draw_line(178 + 104, 126, 208 + 104, 126, 0xFF);
        draw_line(208 + 104, 126, 208 + 104, 134, 0xFF);
        draw_line(178 + 104, 134, 208 + 104, 134, 0xFF);
        draw_line(178 + 104, 134, 178 + 104, 140, 0xFF);
        draw_line(178 + 104, 140, 208 + 104, 140, 0xFF);

        switch (rrr) {
        case 1:
            one2(254, 100);
            break;
        case 2:
            two2(254, 100);
            break;
        case 3:
            three2(254, 100);
            break;
        case 4:
            four2(254, 100);
            break;
        case 5:
            five2(254, 100);
            break;
        case 6:
            six2(254, 100);
            break;
        case 7:
            seven2(254, 100);
            break;
        case 8:
            eight2(254, 100);
            break;
        case 9:
            nine2(254, 100);
            break;
        case 0:
            zero2(254, 100);
            break;
        default:
            one2(254, 100);
            break;
        }
        switch (rr) {
        case 1:
            one2(226, 100);
            break;
        case 2:
            two2(226, 100);
            break;
        case 3:
            three2(226, 100);
            break;
        case 4:
            four2(226, 100);
            break;
        case 5:
            five2(226, 100);
            break;
        case 6:
            six2(226, 100);
            break;
        case 7:
            seven2(226, 100);
            break;
        case 8:
            eight2(226, 100);
            break;
        case 9:
            nine2(226, 100);
            break;
        case 0:
            zero2(226, 100);
            break;
        default:
            one2(226, 100);
            break;
        }
        switch (r) {
        case 1:
            one2(150, 100);
            break;
        case 2:
            two2(150, 100);
            break;
        case 3:
            three2(150, 100);
            break;
        case 4:
            four2(150, 100);
            break;
        case 5:
            five2(150, 100);
            break;
        case 6:
            six2(150, 100);
            break;
        case 7:
            seven2(150, 100);
            break;
        case 8:
            eight2(150, 100);
            break;
        case 9:
            nine2(150, 100);
            break;
        case 0:
            zero2(150, 100);
            break;
        default:
            one2(150, 100);
            break;
        }
        switch (l) {
        case 1:
            one2(122, 100);
            break;
        case 2:
            two2(122, 100);
            break;
        case 3:
            three2(122, 100);
            break;
        case 4:
            four2(122, 100);
            break;
        case 5:
            five2(122, 100);
            break;
        case 6:
            six2(122, 100);
            break;
        case 7:
            seven2(122, 100);
            break;
        case 8:
            eight2(122, 100);
            break;
        case 9:
            nine2(122, 100);
            break;
        case 0:
            zero2(122, 100);
            break;
        default:
            one2(122, 100);
            break;
        }
        switch (ll) {
        case 1:
            one2(46, 100);
            break;
        case 2:
            two2(46, 100);
            break;
        case 3:
            three2(46, 100);
            break;
        case 4:
            four2(46, 100);
            break;
        case 5:
            five2(46, 100);
            break;
        case 6:
            six2(46, 100);
            break;
        case 7:
            seven2(46, 100);
            break;
        case 8:
            eight2(46, 100);
            break;
        case 9:
            nine2(46, 100);
            break;
        case 0:
            zero2(46, 100);
            break;
        default:
            one2(46, 100);
            break;
        }
        switch (lll) {
        case 1:
            one2(18, 100);
            break;
        case 2:
            two2(18, 100);
            break;
        case 3:
            three2(18, 100);
            break;
        case 4:
            four2(18, 100);
            break;
        case 5:
            five2(18, 100);
            break;
        case 6:
            six2(18, 100);
            break;
        case 7:
            seven2(18, 100);
            break;
        case 8:
            eight2(18, 100);
            break;
        case 9:
            nine2(18, 100);
            break;
        case 0:
            zero2(18, 100);
            break;
        default:
            one2(18, 100);
            break;
        }
    } else {
        //mode 1, mode 5, mode 6
        if (mode == 5) {
            FILL_PIXEL(120, 110, 0xFF, 0xFF, 0xFF);
            FILL_PIXEL(120, 130, 0xFF, 0xFF, 0xFF);
        } else {
            //mode 1, mode 6
            FILL_PIXEL(120, 110, 0xFF, 0xFF, 0xFF);
            FILL_PIXEL(120, 130, 0xFF, 0xFF, 0xFF);
            FILL_PIXEL(200, 110, 0xFF, 0xFF, 0xFF);
            FILL_PIXEL(200, 130, 0xFF, 0xFF, 0xFF);
        }
        switch (rrr) {
        case 1:
            one(260);
            break;
        case 2:
            two(260);
            break;
        case 3:
            three(260);
            break;
        case 4:
            four(260);
            break;
        case 5:
            five(260);
            break;
        case 6:
            six(260);
            break;
        case 7:
            seven(260);
            break;
        case 8:
            eight(260);
            break;
        case 9:
            nine(260);
            break;
        case 0:
            zero(260);
            break;
        default:
            one(260);
            break;
        }
        switch (rr) {
        case 1:
            one(220);
            break;
        case 2:
            two(220);
            break;
        case 3:
            three(220);
            break;
        case 4:
            four(220);
            break;
        case 5:
            five(220);
            break;
        case 6:
            six(220);
            break;
        case 7:
            seven(220);
            break;
        case 8:
            eight(220);
            break;
        case 9:
            nine(220);
            break;
        case 0:
            zero(220);
            break;
        default:
            dot();
            break;
        }
        switch (r) {
        case 1:
            one(180);
            break;
        case 2:
            two(180);
            break;
        case 3:
            three(180);
            break;
        case 4:
            four(180);
            break;
        case 5:
            five(180);
            break;
        case 6:
            six(180);
            break;
        case 7:
            seven(180);
            break;
        case 8:
            eight(180);
            break;
        case 9:
            nine(180);
            break;
        case 0:
            zero(180);
            break;
        default:
            one(180);
            break;
        }
        switch (l) {
        case 1:
            one(140);
            break;
        case 2:
            two(140);
            break;
        case 3:
            three(140);
            break;
        case 4:
            four(140);
            break;
        case 5:
            five(140);
            break;
        case 6:
            six(140);
            break;
        case 7:
            seven(140);
            break;
        case 8:
            eight(140);
            break;
        case 9:
            nine(140);
            break;
        case 0:
            zero(140);
            break;
        default:
            one(140);
            break;
        }
        switch (ll) {
        case 1:
            one(100);
            break;
        case 2:
            two(100);
            break;
        case 3:
            three(100);
            break;
        case 4:
            four(100);
            break;
        case 5:
            five(100);
            break;
        case 6:
            six(100);
            break;
        case 7:
            seven(100);
            break;
        case 8:
            eight(100);
            break;
        case 9:
            nine(100);
            break;
        case 0:
            zero(100);
            break;
        default:
            one(100);
            break;
        }
        switch (lll) {
        case 1:
            one(60);
            break;
        case 2:
            two(60);
            break;
        case 3:
            three(60);
            break;
        case 4:
            four(60);
            break;
        case 5:
            five(60);
            break;
        case 6:
            six(60);
            break;
        case 7:
            seven(60);
            break;
        case 8:
            eight(60);
            break;
        case 9:
            nine(60);
            break;
        case 0:
            zero(60);
            break;
        default:
            one(60);
            break;
        }
    }
    wait_for_vsync();
}
