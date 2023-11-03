#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

// WAIT TIME
#define WAIT_1_MSEC 1000
#define WAIT_2_MSEC 100

// LEDS
#define MY_LED_1 NRF_GPIO_PIN_MAP(0, 6)
#define LED_2_R NRF_GPIO_PIN_MAP(0, 8)
#define LED_2_G NRF_GPIO_PIN_MAP(1, 9)
#define LED_2_B NRF_GPIO_PIN_MAP(0, 12)

// SWITCH
#define SW_BUTTON NRF_GPIO_PIN_MAP(1, 6)

// nRF ID and LEDS
const int ID[] = {7, 3, 2, 4};
const uint32_t LEDS[] = {MY_LED_1, LED_2_R, LED_2_G, LED_2_B};

void init_switch(){
    nrf_gpio_cfg_input(SW_BUTTON, NRF_GPIO_PIN_PULLUP);
}

void init_led(const uint32_t led){
    nrf_gpio_cfg_output(led);
    nrf_gpio_pin_write(led, 1);
}

void ton_led(int i){
    nrf_gpio_pin_write(LEDS[i], 0);
}

void toff_led(int i){
    nrf_gpio_pin_write(LEDS[i], 1);
}

bool led_state(int i){
    return nrf_gpio_pin_read(LEDS[i]);
}

bool press_switch(){
    return !nrf_gpio_pin_read(SW_BUTTON);
}


int main(void)
{
    for (int i = 0; i < LEDS_NUMBER; i++){
        init_led(LEDS[i]);
    }
    init_switch();

    int led = 0;
    int counter = 0;
    int hold = 0;
    while(1){
        if (led == 3 && counter == ID[led]){
            led = 0;
            counter = 0;
        }
        if (counter == ID[led]){
            led += 1;
            counter = 0;
        }
        while(press_switch()){
            if (!led_state(led)){
                ton_led(led);
            }
            if (counter == ID[led]){
                break;
            }
            if (hold == WAIT_1_MSEC){
                counter++;
                hold = 0;
            }
            hold += WAIT_2_MSEC;
            nrf_delay_ms(WAIT_2_MSEC);
        }
        if (!led_state(led)){
            toff_led(led);
        }
        nrf_delay_ms(WAIT_1_MSEC);
    }
    return 0;
}
