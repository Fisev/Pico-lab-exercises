#include "pico/stdlib.h"

#define LED_PIN 25

int led_value = 0;

bool repeating_timer_callback(struct repeating_timer *t){
    led_value = 1- led_value;
    gpio_put(LED_PIN, led_value); 
    return true;
}

int main(){
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    struct repeating_timer timer;
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);

    while(true){
        sleep_ms(6000);
    }
}