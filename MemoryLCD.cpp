#include "MemoryLCD.h"
#include "stm32f4xx_spi.h"

MemoryLCD::MemoryLCD() : Adafruit_GFX(LCDWIDTH, LCDHEIGHT) {}

void MemoryLCD::begin(void) {

    GPIO_InitTypeDef GPIO_InitStruct;
    SPI_InitTypeDef SPI_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

//60Hz PWM on E9 for EXTCOMIN

    /* TIM1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* GPIOE clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    /* TIM1 channel 2 pin (PE.9) configuration */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* Connect TIM pins to AF2 */
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);

    TIM_TimeBaseStructInit (&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/ 6000) - 1; //CPU -> 6khz (0-65536 range)
    TIM_TimeBaseStructure.TIM_Period = (6000 / 60 / 3) -1; //6khz -> 60Hz (no idea where the 3 comes from)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1 , &TIM_TimeBaseStructure);

    // PWM1 Mode configuration: Channel1
    // Edge -aligned; not single pulse mode
    TIM_OCStructInit (& TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(TIM1 , &TIM_OCInitStructure);

    TIM_BDTRInitTypeDef bdtr;
    TIM_BDTRStructInit(&bdtr);
    bdtr.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM1, &bdtr);

    // Enable Timer Interrupt and Timer
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); //
    TIM_Cmd(TIM1 , ENABLE);

    TIM_SetCompare1(TIM1, (6000 / 60 / 3) /5); //20% on

// setup SPI A5 = SCLK, A7 = SI, E7 = Chipselect (CSC)

    // enable clock for used IO pins
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // connect SPI1 pins to SPI alternate function
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

    // enable clock for used IO pins
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    // Configure the chip select pin
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIOE->BSRRH |= GPIO_Pin_7; // unset PE7

    // enable peripheral clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // SPI frequency is APB2 frequency / 64
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI1, &SPI_InitStruct);
    SPI_Cmd(SPI1, ENABLE);

//Setup GPIO for A1 = DISP and A2 = EXTMODE

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIOA->BSRRL |= GPIO_Pin_2; //A2 high / EXTCOM on
    delayMicroseconds(PWRUP_EXTCOMIN_DELAY);

    GPIOA->BSRRL |= GPIO_Pin_1; //A1 high / display on
    delayMicroseconds(PWRUP_DISP_DELAY);
}

void MemoryLCD::write(unsigned char line, unsigned char numLines, unsigned char *lines) {
    GPIOE->BSRRL |= GPIO_Pin_7;
    delayMicroseconds(SCS_HIGH_DELAY);
    SPI1->DR = 0b10000000;
    while( !(SPI1->SR & SPI_I2S_FLAG_TXE) );
    for(unsigned char x=0; x<numLines; x++) {
        //SPI LSB toggling loks expensive lets just do it outself
        unsigned char b = ++line;
        b = ((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
        SPI1->DR = b;
        while( !(SPI1->SR & SPI_I2S_FLAG_TXE) );
        for(unsigned char y=0; y<LCDWIDTH/8; y++) {
            SPI1->DR = *(lines++);
            while( !(SPI1->SR & SPI_I2S_FLAG_TXE) );
        }
        SPI1->DR = 0x00;
        while( !(SPI1->SR & SPI_I2S_FLAG_TXE) );
    }
    SPI1->DR = 0x00;
    while( !(SPI1->SR & SPI_I2S_FLAG_TXE) );
    delayMicroseconds(SCS_LOW_DELAY);
    GPIOE->BSRRH |= GPIO_Pin_7;
    delayMicroseconds(INTERFRAME_DELAY);
}

void MemoryLCD::flush() {
    write(0, LCDHEIGHT, buffer);
}

void MemoryLCD::drawPixel(int16_t x, int16_t y, uint16_t color) {
    int i = ((y*(LCDWIDTH/8)))+x/8;
    int shift = 7-(x % 8);
    if (color) {
        buffer[i] |= (1 << shift);
    } else {
        buffer[i] &= ~(1 << shift);
    }
}
