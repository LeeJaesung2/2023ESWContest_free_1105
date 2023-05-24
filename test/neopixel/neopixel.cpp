static char VERSION[] = "XX.YY.ZZ";

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>
#include <getopt.h>
#include <iostream>


#include <ws2811/clk.h>
#include <ws2811/gpio.h>
#include <ws2811/dma.h>
#include <ws2811/pwm.h>
#include <ws2811/version.h>
#include <ws2811/ws2811.h>


// defaults for cmdline options
#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN_18             18
#define GPIO_PIN_23             23
#define DMA                     10
#define STRIP_TYPE              WS2811_STRIP_GBR     // WS2812/SK6812RGB integrated chip+leds
#define LED_COUNT               9

int led_count = LED_COUNT;
int clear_on_exit = 0;

ws2811_t ledstring =
{
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel =
    {
        [0] =
        {
            .gpionum = GPIO_PIN_18,
            .invert = 0,
            .count = LED_COUNT,
            .strip_type = STRIP_TYPE,
            .brightness = 255,
        },
        [1] =
        {
            .gpionum = GPIO_PIN_23,
            .invert = 0,
            .count = LED_COUNT,
            .strip_type = STRIP_TYPE,
            .brightness = 255,
        },
    },
};

ws2811_led_t *matrix1;
ws2811_led_t *matrix2;

static uint8_t running = 1;

void matrix_render(void) {
    int x;

    for (x = 0; x < LED_COUNT; x++) {
        
        ledstring.channel[0].leds[x] = matrix1[x]; // Pin 18
        ledstring.channel[1].leds[x] = matrix2[x]; // Pin 15 
        
    }
}

void matrix_clear(void) {
    int x;

    for (x = 0; x < LED_COUNT; x++) {
        matrix1[x] = 0;
    }
}


void matrix_mode1(void){
    for(int x=0; x<led_count; x++){
        matrix1[x] = 0x00000020; //Red color
        matrix2[x] = 0x00200000; //Blue color 
    }
}

void matrix_mode2(void){
    for(int x=0; x<led_count; x++){
        matrix1[x] = 0x00200000; //Blue color
        matrix2[x] = 0x00000020; //Red color
    }
}


int main(int argc, char *argv[]) {
    ws2811_return_t ret;

    sprintf(VERSION, "%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

    matrix1 = (ws2811_led_t*)malloc(sizeof(ws2811_led_t) * LED_COUNT);
    matrix2 = (ws2811_led_t*)malloc(sizeof(ws2811_led_t) * LED_COUNT);

    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
        return ret;
    }

    while (running) {

        matrix_mode1();
        matrix_render();

        if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
            fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
            break;
        }

        usleep(1000000);

        matrix_mode2();
        matrix_render();

        if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
            fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
            break;
        }

        usleep(1000000);
    }

    if (clear_on_exit) {
        matrix_clear();
        matrix_render();
        ws2811_render(&ledstring);
    }

    ws2811_fini(&ledstring);

    printf("\n");
    return ret;
}

