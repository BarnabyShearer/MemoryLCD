#ifndef MEMORY_LCD_H
#define MEMORY_LCD_H

#include <libs/Adafruit-GFX-Library/Adafruit_GFX.h>

#define LCDWIDTH		        128
#define LCDHEIGHT		        128
#define PWRUP_DISP_DELAY	    40
#define PWRUP_EXTCOMIN_DELAY	40
#define SCS_HIGH_DELAY		    3
#define SCS_LOW_DELAY		    1
#define INTERFRAME_DELAY    	1

class MemoryLCD: public Adafruit_GFX {
    public:
        MemoryLCD();
        void begin();
        void write(unsigned char line, unsigned char num, unsigned char *lines);
        void flush();
        void drawPixel(int16_t x, int16_t y, uint16_t color);
    private:
        unsigned char buffer[LCDWIDTH*LCDHEIGHT/8];
};

#endif
