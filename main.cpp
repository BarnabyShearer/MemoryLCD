/*
 * STM32F4 Sharp Memory LCD test
 *
 * Copyright 2014 <b@Zi.iS>
 * License GPLv2
 */

#include "stm32f4xx.h"
#include "MemoryLCD.h"

MemoryLCD lcd;

void setup() {
    lcd.begin();
    lcd.setTextColor(0);
    lcd.setTextSize(1);
    lcd.setTextWrap(1);
}

void loop() {
    puts("Loop");
    lcd.fillScreen(1);
    lcd.setCursor(0, 0);
    lcd.println("\n Adafruit GFX for\n Sharp Memory LCD\n");
    lcd.flush();
    delay(2000);
    lcd.fillScreen(1);
    for(int x=0;x<lcd.width()/2;x=x+4) {
        lcd.drawRect(x, x, lcd.width() - 2 * x, lcd.height() - 2 * x, 0);
        lcd.drawCircle(lcd.width() /2 , lcd.height() /2, x, 0);
        lcd.flush();
    }
    delay(1000);
}

int main(int argc, char* argv[]) {

    SystemInit();
    setup();

    while (1) {
        loop();
    }
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line) {
    printf("Wrong parameters value: file %s on line %lu\r\n", file, line);
}

#endif
