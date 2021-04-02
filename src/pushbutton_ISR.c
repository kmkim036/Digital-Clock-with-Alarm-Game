#include "address_map_nios2.h"
#include "key.h"

static volatile int* KEY_ptr = (int*)KEY_BASE;
static volatile int* SW_ptr = (int*)SW_BASE;

void pushbutton_ISR()
{
    int press = *(KEY_ptr + 3);
    *(KEY_ptr + 3) = press;

    int sw = *(SW_ptr);
    if (press == 1) {
        key0(sw);
    } else if (press == 2) {
        key1();
    } else if (press == 4) {
        key2();
    } else {
        key3();
    }
}
