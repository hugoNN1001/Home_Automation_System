//These numbers are the bit patterns for lighting up the respective LED    
#define LED1	1
#define LED2	2
#define LED3	4
#define LED4	8              

extern void LEDCycle_Init(void);
extern void LEDCycle(unsigned int value);
extern void delay(void);

// Memory mapped register addresses
#define ADC_CR      (*(volatile unsigned int*)0xE0034000)  // ADC Control Register
#define ADC_DR1     (*(volatile unsigned int*)0xE0034014)  // AD0.1
#define PINSEL1    (*(volatile unsigned int *)0xE002C004)
#define PINSEL7    (*(volatile unsigned int *)0xE002C01C)
#define GPIO3DIR   (*(volatile unsigned int *)0x3FFFC064)
#define GPIO3SET   (*(volatile unsigned int *)0x3FFFC058)
#define GPIO3CLR   (*(volatile unsigned int *)0x3FFFC05C)

// Define tricolour LED pin masks on PORT 3
#define RED_LED     (1 << 16)   // P3.16
#define GREEN_LED   (1 << 17)   // P3.17
#define BLUE_LED    (1 << 18)   // P3.18

void init_ADC(void);
unsigned int light_sensor(void);
void init_LEDs(void);
void set_LED_color(unsigned int light_val);

int main(void) {
    unsigned int value_of_light;

    init_ADC();  // Setting up ADC and pin for analog input (AD0.1)

    while (1) {
			
				init_ADC();     // Setup ADC for AD0.1
				init_LEDs();
			
        value_of_light = light_sensor();  // Polling light sensor value from AD0.1

        set_LED_color(value_of_light);    // Display state of the blind with LED
    }
}

// Initializing ADC for channel AD0.1
void init_ADC(void) {
    // Setting P0.24 pin as AD0.1 via PINSEL1 (bits 16�17 = 01)
    PINSEL1 &= ~(0x3 << 16);   // move 0x3 (11) to bits 16 and 17 then invert to 00; used & to only work on bits 16 and 17
    PINSEL1 |=  (0x1 << 16);   // producing bits 0b01 in bits 16 and 17, setting bit 16 to 1 and 17 to 0

	// ConfigurING ADC_CR (A/D CR) to select channel 1, set clock divider, and power up ADC
    ADC_CR = (1 << 1)        // setting bit 1 = 1 to select channel AD0.1 (Bit 0 - AD0.0, Bit 1 - ADO.1, ...)
						// this line of code slows down the AC clock so it remains within its operational limits
           | (10 << 8)       // Bits 8-15 in ADC_CR control the clock divider; ADC clock = PCLK = (CLKDIV + 1); CLKDIV = 10
           | (1 << 21);      // Bit 21 is the Power Down Control (PDC) set to 1 so its powered up and ready to convert
}

// Reading the light level from AD0.1 and returning a 10-bit value
unsigned int light_sensor(void) {
    unsigned int adc_value; // holds the 10-bit result from the ADC

    // ADC conversion
    ADC_CR &= ~(7 << 24);     // Bits 24-26 control how and when the ADC starts conversion; 7 is 0b111 << 24, targetting 
		// bits 24-26; ~ clears the bits; & applies the mask
    ADC_CR |=  (1 << 24);     // Bit 24 = 1 setting START to 0b001 - start conversion immediately 

    // Waiting for the conversion to be completed
    while (!(ADC_DR1 & (1U << 31))); // AD0DR1 result register; bit 31 is the DONE bit; while - wait in the loop until conversion
		// is done where bit 31 = 1 means conversion completed

    // Extracting the 10-bit result 
    adc_value = (ADC_DR1 >> 6) & 0x3FF; // 10 bit result is stored in bits 6-15 of AD0DR1; adc_value is right justified 
		// due to >> 6; 0x3FF masks the upper bits to only retain the 10 bit result 

    return adc_value;
}

void init_LEDs(void) {
    // Clear P3.16�18 function bits (set as GPIO)
    PINSEL7 &= ~((3 << 0) | (3 << 2) | (3 << 4));

    // Set P3.16�18 as output
    GPIO3DIR |= (1 << 16) | (1 << 17) | (1 << 18);
}

void set_LED_color(unsigned int light_val) {
    // Turn off all LEDs
    GPIO3CLR = (1 << 16) | (1 << 17) | (1 << 18);

    if (light_val < 300) {
        GPIO3SET = (1 << 16);  // RED
    } else if (light_val < 700) {
        GPIO3SET = (1 << 17);  // GREEN
    } else {
        GPIO3SET = (1 << 18);  // BLUE
    }
}

