#include <stdio.h>
#include "pico/stdlib.h"
#include <inttypes.h>

#define BUTTON_PIN 10
size_t nast_hrana = 0;
size_t sest_hrana = 0;
size_t pocetZmacknutiTl = 0;
size_t prechodovyDejUstal = 1;

int_fast64_t alarm_callback(alarm_id_t id, void * user_data){
    prechodovyDejUstal = 1;
    if(user_data == &sest_hrana)
        printf("Pocet zmacknuti tlacitka: %u \n", ++pocetZmacknutiTl);
    return 0;
}

void callback(uint gpio, uint32_t events){
    if(events == 4){                    // pro pripad sestupne hrany (zmacknuti tlacitka)
        if (prechodovyDejUstal){
            add_alarm_in_ms(100, alarm_callback, &sest_hrana, true);
            prechodovyDejUstal = 0;
        }
    }

    if(events == 8){                // pro pripad nastupne hrany 
      if (prechodovyDejUstal){
            add_alarm_in_ms(100, alarm_callback, &nast_hrana, true);
            prechodovyDejUstal = 0;
        }
    }
}

int main(){
    stdio_init_all();       //pro tisk na konzoli je nutna tato funkce -> inicializace UART

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_set_pulls(BUTTON_PIN, true, false);

    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &callback);   //nastavi interrupt pri zmacknuti tlacitka

    while(true){
        sleep_ms(1000);
    }
}