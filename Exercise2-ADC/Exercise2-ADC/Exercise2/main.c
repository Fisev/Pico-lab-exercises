#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/dma.h"

#define CAPTURE_SIZE 3700
#define ADC_CLK 48000000
#define ADC_MAX_DEVIDER (1 << 16) -1 //maximum

uint8_t capture_buffer[CAPTURE_SIZE];

int main(){
    stdio_init_all();
    adc_init();   //Inicializace ADC HW
    adc_gpio_init(26); // Inicializace GPIO pro analog: high-impedance, no pullups, disable digital input buffer
    adc_select_input(0); // Vyber v analogoveho mux vstup 0 (GPIO 26)
    adc_set_clkdiv(ADC_MAX_DEVIDER);  //Dělička frakvence pro ADC - perioda 1,36ms
    adc_fifo_setup(
        true, //  Zapis kazdou dokoncenou konverzi do sample FIFO
        true, //Enable DMA data reguest (DREG) 
        1, //DREG (and IRQ) je nastaven pokud je alespon jeden vzorek 
        false, //Pokud je povoleno bit 15 je error flag pro kazdy vzorek
        true //Shift each sample to 8 bits when pushing to FIFO
    );

    int dma_chan = 0;
    dma_channel_config c = dma_channel_get_default_config(dma_chan);   //Defaultní konfigurace
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);      //Prenos po 8 bitech
    channel_config_set_read_increment(&c, false); 
    channel_config_set_write_increment(&c, true); 
    channel_config_set_dreq(&c, DREQ_ADC);  //Aby DMA spustil prenos pri vystaveni DREQ_ADC    
    dma_channel_configure(
        dma_chan,   //Kanal, ktery ma byt konfigurovan
        &c,         //Vytvorena konfigurace
        capture_buffer,    //Cil
        &adc_hw->fifo,  //Zdroj
        CAPTURE_SIZE,  // Pocet prenosu
        true            //Zacit hned
    );

    sleep_ms(5000);
    printf("Starting capture\n");
    adc_run(true); //Kontinualni beh adc

    dma_channel_wait_for_finish_blocking(dma_chan);     //Ceka az se dokonci prenos DMA
    printf("Capture finished\n");
    adc_run(false);
    adc_fifo_drain();   //Pocka az se dokonci prevod a vysledek smaze

    for (int i = 0; i < CAPTURE_SIZE; ++i) {
        const float conversion_factor = 3.3f / (1 << 8);  // 8 bitovy ADC prevod, max. hodnota je 3.3 V a min 0 V
        printf("0x%x, Napeti: %f V, Index [%d]\n", capture_buffer[i], capture_buffer[i] * conversion_factor, i);
    }
    while(true)
        sleep_ms(1000);
}