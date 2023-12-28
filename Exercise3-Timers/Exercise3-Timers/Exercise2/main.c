#include <stdio.h>
#include "pico/stdlib.h"
#include <inttypes.h>

#define BUTTON_PIN 10
size_t pocetZmacknutiTl = 0;

void callback(uint gpio, uint32_t events){
    printf("Pocet zmacknuti tlacitka: %u \n", ++pocetZmacknutiTl);
}

int main(){
    stdio_init_all();       //pro tisk na konzoli je nutna tato funkce -> inicializace UART

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_set_pulls(BUTTON_PIN, true, false);

    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, callback);

    while(true){
        sleep_ms(1000);
    }
}