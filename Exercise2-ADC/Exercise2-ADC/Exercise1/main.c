#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

int main(){
    stdio_init_all();
    printf("ADC Example, measuring GPIO26\n");
 
    //Inicializace ADC HW
    adc_init();
 
    // Inicializace GPIO pro analog: high-impedance, no pullups, disable digital input buffer
    adc_gpio_init(26);
    // Vyber v analogoveho mux vstup 0 (GPIO 26)
    adc_select_input(0);

 
    while (true) {
        // 12 bitove prevody, max. hodnota je 3.3 V, min je 0 V
        const float conversion_factor = 3.3f / (1 << 12);
        uint16_t result = adc_read();
        printf("Raw value: 0x%x, voltage: %f V\n", result, result * conversion_factor);
        sleep_ms(500);
    }
}