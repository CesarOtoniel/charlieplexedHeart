/*
  This code goes through different loops as the button makes contact with ground.
  This program requires deactivating the reset button and a High Voltage Programmer to set the fuses. 
  It will compile in MPLABX and Arduino IDE with the ATTinyCore library by SpenceKonde. 
  The program was tested at 1MHz frequency on an ATtiny85 microcontroller.
  

  ATTiny Core: https://github.com/SpenceKonde/ATTinyCore

  Written by Cesar O Guevara.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include <avr/io.h>
#include <util/delay.h>

// Define port I/O  states for charlieplexing
#define LED_A_ON 0b00001 //Controls LED 1,9,11,13
#define LED_B_ON 0b00010 //Controls LED 2,3,15,17
#define LED_C_ON 0b00100 //Controls LED 4,5,10,19
#define LED_D_ON 0b01000 //Controls LED 6,7,12,16
#define LED_E_ON 0b10000 //Controls LED 8,14,18,20
#define LED_OFF 0b00000
#define FIRST_5_BITS_MASK 0b00011111 // Mask to write only on PB0-PB4
#define BUTTON_PIN PB5 //PB5 for pushbutton
#define N_LED 8
volatile int count = 0;
// Define LED combinations for charlieplexing
const uint8_t LED_ADDRESS[] = {
    LED_A_ON,LED_B_ON, LED_B_ON ,LED_C_ON,   //0-3 
    LED_C_ON,LED_D_ON, LED_D_ON ,LED_E_ON,   
    LED_A_ON,LED_C_ON, LED_A_ON ,LED_D_ON,   
    LED_A_ON,LED_E_ON, LED_B_ON ,LED_D_ON,   
    LED_B_ON,LED_E_ON, LED_C_ON ,LED_E_ON};  //16-19

const uint8_t LED_IO[] = {
    LED_A_ON | LED_B_ON, LED_A_ON | LED_B_ON, LED_B_ON | LED_C_ON, LED_B_ON | LED_C_ON,
    LED_C_ON | LED_D_ON, LED_C_ON | LED_D_ON, LED_D_ON | LED_E_ON, LED_D_ON | LED_E_ON,
    LED_A_ON | LED_C_ON, LED_A_ON | LED_C_ON, LED_A_ON | LED_D_ON, LED_A_ON | LED_D_ON,
    LED_A_ON | LED_E_ON, LED_A_ON | LED_E_ON, LED_B_ON | LED_D_ON, LED_B_ON | LED_D_ON,
    LED_B_ON | LED_E_ON, LED_E_ON | LED_B_ON, LED_E_ON | LED_C_ON, LED_E_ON | LED_C_ON};

void turn_on_led(uint8_t led_combination) {
    // Disable all pins
    PORTB &= ~FIRST_5_BITS_MASK;
    DDRB &= ~FIRST_5_BITS_MASK;
    DDRB |= (LED_IO[led_combination]& FIRST_5_BITS_MASK);
    PORTB |= (LED_ADDRESS[led_combination]& FIRST_5_BITS_MASK);
    
}

void turn_off_led() {
    // Disable all pins
    PORTB &= ~FIRST_5_BITS_MASK;
}

void delay_ms(uint16_t milliseconds) {
    while (milliseconds--) {
        _delay_ms(1);
    }
}

void delay_us(uint16_t us) {
    while (us--) {
        _delay_us(1);
    }
}

void circleonebyone(uint8_t speed) {
    // Cycle through all LED one by one  
    if (count < N_LED) {
        turn_on_led(count);
        count++;
        if (N_LED == count) {
          count= 0;
        }
    if (speed>0){delay_ms(speed);}
    }
} 

void loading(){  
    // Cycle through all LED one by one  
    for (int j = 0; j<250; j++){
    for (int i = 0; i<N_LED; i++) {
        if(count==i){turn_off_led();}else{turn_on_led(i);}
        }
        turn_off_led();
        }
    count++;
    if(count==N_LED){count=0;}
}               


void heartBeat(){
    // Cycle through all LED one by one  
    for (int j = 0; j<255; j++){
    for (int i = 0; i<N_LED; i++) {
        turn_on_led(i);
        }
        turn_off_led();
        delay_us(j);
        }
        delay_ms(5);
}


void blink(uint8_t speed){
    // Cycle through all LED one by one  
    for (int j = 0; j<250; j++){
    for (int i = 0; i<N_LED; i++) {
        turn_on_led(i);
        }
        turn_off_led();
        }
    if (speed>0){delay_ms(speed);}
}
void reverse(uint8_t speed){
    turn_on_led(count);
    if(count==0){count=N_LED-1;}else{count--;}
    if (speed>0){delay_ms(speed);}
    }


int main(void) {
//    DDRB &= ~(1 << BUTTON_PIN);  // Set as input
//    PORTB |= (1 << BUTTON_PIN);   // Enable pull-up resistor
    int option = 1;
    while(1){
    while (PINB & (1 << BUTTON_PIN)) {
            switch (option){
        
            case 0:
                heartBeat();
                break;
        
             case 1:
                circleonebyone(125);
                break;
        
            case 2:
                circleonebyone(10);
                break;
        
            case 3:
                circleonebyone(0);  
                break;
        
            case 4:
                reverse(50);
                break;
        
            case 5:
                loading();
                break;
            
            case 6:
                blink(100);
                break;
       
            }   
        }
    if(option<6){option++;}else{option=0;}
    while(!(PINB & (1 << BUTTON_PIN))){_delay_ms(10);}
    }
        return 1;
    }

