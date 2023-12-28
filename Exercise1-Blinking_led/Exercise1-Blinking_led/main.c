#include <stdio.h>
#include "pico/stdlib.h"

int main(){
    const uint led_pin = 25;
    const uint button_pin = 10;
    unsigned long mask_for_led_pin = 1u << led_pin;
    unsigned long mask_for_button_pin = 1u << button_pin;

    sio_hw->gpio_oe_clr = mask_for_led_pin;  //zakazani outputenable v sio registrech str. 29 datasheet
    sio_hw->gpio_clr = mask_for_led_pin;     //az se povoli v sio pin, tak aby byl vystup 0 str. 29 datasheet
    iobank0_hw->io[led_pin].ctrl = 5; //nastavi v GPIO5_CTR regisru FUNCSEL aby byl gpio ovladan pomoci sio   
    sio_hw->gpio_oe_set = mask_for_led_pin;  //povoleni outputenable v sio registrech str. 47 datasheet
    
    sio_hw->gpio_oe_clr = mask_for_button_pin;  //zakazani outputenable v sio registrech str 29 datasheet
    sio_hw->gpio_clr = mask_for_button_pin;     //az se povoli v sio pin, tak aby byl vystup 0 str. 29 datasheet
    iobank0_hw->io[led_pin].ctrl = 5; //nastavi v GPIO5_CTR regisru FUNCSEL aby byl gpio ovladan pomoci sio
    
    padsbank0_hw->io[button_pin] = 0x00000040; //nastaveni v PADS_BANK0[GPIO10] IE (Input enable) do 1
    padsbank0_hw->io[button_pin] |= 0x00000008;  //nastaveni v PADS_BANK0[GPIO10] PUE (Pull up enable) do 1

    while(true){
        if ((mask_for_button_pin) & sio_hw->gpio_in){      
            sio_hw->gpio_clr = mask_for_led_pin;            //pokud je pin v logicke 1 ledka nesviti
        }
        else{
            sio_hw->gpio_set = mask_for_led_pin; //pokud je pin prizemnen ledka sviti
        }
    }
};