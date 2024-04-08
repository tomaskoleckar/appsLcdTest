// **************************************************************************
//
//               Demo program for APPS labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2022
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Main program for LCD module
//
// **************************************************************************

#include "mbed.h"
#include "lcd_lib.h"
#include "graph_class.hpp"
#include "text_class.hpp"

// two dimensional array with fixed size font
extern uint8_t g_font8x8[256][8];

DigitalOut g_led_PTA1( PTA1, 0 );
DigitalOut g_led_PTA2( PTA2, 0 );

DigitalIn but_9( PTC9 );
DigitalIn but_10( PTC10 );
DigitalIn g_but_PTC11( PTC11 );
DigitalIn g_but_PTC12( PTC12 );

int font_width = 12;
int font_height = 16;
int g_refresh = 0;
int g_refresh1 = 0;

Point2D start{LCD_WIDTH/2, LCD_HEIGHT/2};
Point2D start_text = start;

void buttons(){
	if(but_9 == 0){
		g_refresh = 1;
		return;
	}
	if(but_10 == 0){
			g_refresh1 = 1;
			return;
		}
}

int main()
{
	lcd_init();				// LCD initialization
	Ticker button_ticker;
	button_ticker.attach(buttons, 20ms);

	Point2D stred {160,120};

	Point2D stredH {140,115};

	Point2D stredK {170,115};

	RGB bila {255,255,255};
	RGB barvaCH {255, 73, 148};
	RGB barvaK {148, 18, 116};
	RGB cerna {0,0,0};

	Circle x_m {stred, 100, barvaK, cerna};

	Character H {stredH, 'H', barvaCH, cerna, font_width, font_height};

	Character K {stredK, 'K', barvaCH, cerna, font_width, font_height};


	Point2D A {20, 220};
	Point2D B {300, 220};
	Point2D C {300, 20};
	Point2D D {20, 20};

	Line AB {A, B, bila, cerna};
	Line BC {C, B, bila, cerna};
	Line CD {D, C, bila, cerna};
	Line DA {D, A, bila, cerna};
	Line AC {A, C, bila, cerna};
	Line BD {D, B, bila, cerna};

	H.draw();
	K.draw1();

	while (1){
			if(g_refresh){
				g_refresh = 0;
				x_m.draw();
			}
			if(g_refresh1){
				g_refresh1 = 0;
				AB.draw();
				BC.draw();
				CD.draw();
				DA.draw();
				AC.draw();
				BD.draw();
				}
		}

	return 0;
}
