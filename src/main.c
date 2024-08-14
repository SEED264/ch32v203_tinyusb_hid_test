#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "debug.h"
#include "bsp/board_api.h"
#include "tusb.h"
#include "variables.h"

volatile uint64_t debug_flags_for_handler = 0;
__attribute__((interrupt)) __attribute__((used))
void NMI_Handler(void) {
    debug_flags_for_handler |= (1 << 0);
}
__attribute__((interrupt)) __attribute__((used))
void HardFault_Handler(void) {
    debug_flags_for_handler |= (1 << 1);
}
__attribute__((interrupt)) __attribute__((used))
void Break_Point_Handler(void) {
    debug_flags_for_handler |= (1 << 4);
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

void tud_mount_cb(void)
{
    printf("tud_mount_cb\n");
}

void tud_umount_cb(void)
{
    printf("tud_umount_cb\n");
}

void tud_suspend_cb(bool remote_wakeup_en)
{
    printf("tud_suspend_cb\n");
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
    printf("tud_resume_cb\n");
}

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
    // printf("tud_hid_report_complete_cb");
}

uint32_t _get_xxhash(uint32_t data, uint32_t seed)
{
    const uint32_t PRIME32_2 = 2246822519U;
    const uint32_t PRIME32_3 = 3266489917U;
    const uint32_t PRIME32_4 = 668265263U;
    const uint32_t PRIME32_5 = 374761393U;
    uint32_t h32 = (uint32_t)data * PRIME32_3;
    h32 += seed + PRIME32_5 + 4U;
    h32 = (h32 << 17) | (h32 >> 15);
    h32 *= PRIME32_4;
    h32 ^= h32 >> 15;
    h32 *= PRIME32_2;
    h32 ^= h32 >> 13;
    h32 *= PRIME32_3;
    h32 ^= h32 >> 16;
    return h32;
}

uint32_t get_xxhash(uint32_t data)
{
    return _get_xxhash(data, 12345U);
}

int main(void) {
    SystemCoreClockUpdate();
    USART_Printf_Init(250000);
    printf("SystemClk:%d\r\n", SystemCoreClock);

    board_init();
    tusb_init();
    tud_init(0);

    uint32_t x = rand();
    uint32_t y = rand();
    uint8_t report[5];
    while(1) {
        tud_task();
        if (tud_hid_ready()) {
            x = get_xxhash(x);
            y = get_xxhash(y);
            report[0] = x & 0xff;
            report[1] = y & 0xff;
            report[2] = (y >> 8) & 0xff;
            report[3] = (y >> 16) & 0xff;
            report[4] = (y >> 24) & 0xff;
            tud_hid_report(0, &report, sizeof(report));
        }
    }
}

