#include "bubbles.h"
#include "lpc24xx.h"   
#include "lcd/lcd_hw.h"
#include "lcd/lcd_grph.h"
#include "lcd/lcd_cfg.h"
#include "lcd/sdram.h"
#include "delay.h"
#include <stdlib.h>

//Location and size of current bubble
int bubble_x;
int bubble_y;
int bubble_r;	

//Init the bubbles
void blow_bubbles(void) {
	//Setup external SDRAM. Used for Frame Buffer
	sdramInit();	
	
	//Setup LPC2478 LCD Controller for our specific LCD
	//lcd_config is defined in lcd/lcd_cfg.h
	lcdInit(&lcd_config); 
	
	//Turn the LCD on
	lcdTurnOn();
	
	//Blow the first bubble
	newbubble();
}

//Attempts to pop a bubble
//Accepts an x and y co-ordinate ranging from 0-255
void pop_bubble(char x, char y) {
	//scale the coordinates to the screens resolution
	int point_x = x*240/255;
	int point_y = y*320/255; 
	
	//calculate the distance from the center of the bubble to the touch point
	int distance = ((point_x - bubble_x)*(point_x - bubble_x) + 
								 (point_y - bubble_y)*(point_y - bubble_y));
	
	//"pop" the bubble if it the distance is less than the radius
	if (distance < bubble_r*bubble_r) {
			//draw a new bubble
			newbubble();
	}

	//for debugging purposes draw dots where touches are registered
	//lcd_point(point_x, point_y, BLACK);
}

//Draw a new bubble
void newbubble() {
	// place and size a random bubble so it does not escape the screen
	bubble_r = 10 + (rand() % 30);
	bubble_x = (bubble_r*2) + (rand() % (240 - bubble_r*4));
	bubble_y = (bubble_r*2) + (rand() % (320 - bubble_r*4));
	
	// draw the bubble on the screen
	lcd_fillScreen(BLUE_BG);	
	lcd_fillcircle(bubble_x, bubble_y, bubble_r, PURPLE);
}
