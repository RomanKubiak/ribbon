#pragma once

#include <TimerOne.h>
#include <WS2812Serial.h>
#include <LedControl.h>
#include <functional>
#include <Bounce2.h>
#include <LibPrintf.h>

#define NUM_LEDS            66
#define LED_PIN             24
#define RED                 0x010000
#define RED_BRIGHT          0x0f0000
#define BLUE                0x000001
#define BLACK               0x000000
#define WHITE               0x101010
#define SDCARD_CS_PIN       10
#define SDCARD_MOSI_PIN     7
#define SDCARD_SCK_PIN      14
#define GRANULAR_MEM_SIZE   12800 // enough for 1160 ms at 44.1 kHz
#define ENCODER_A           3
#define ENCODER_B           2
#define BLACK_BUTTON1       33
#define BLACK_BUTTON2       31
#define SWITCH_BOTTOM       32
#define SWITCH_TOP          30

/*
static const uint8_t value_map_top[] = {
        3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13,
        14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24,
        25, 26, 27, 28, 29, 30,
        31, 32, 33};

static const uint8_t value_map_bottom[] = {
        64, 63, 62, 61, 60,
        59, 58, 57, 56, 55, 54,
        53, 52, 51, 50, 49,
        48, 47, 46, 45, 44, 43,
        42, 41, 40, 39, 38,
        37, 36, 35, 34};

static const uint8_t map_to_value_top(const uint8_t v) {
    return value_map_top[v];
}

static const uint8_t map_to_value_bottom(const uint8_t v) {
    return value_map_bottom[v];
}*/
