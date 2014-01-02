/*
 * STM32F4 Template
 *
 * Copyright 2014 <b@Zi.iS>
 * License GPLv2
 */

#include "stm32f4xx.h"
#include <stdio.h>

void SysTick_Handler(void);

static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;
static void Delay(__IO uint32_t nTime);
extern void _start(void);

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(int argc, char* argv[])
{
    /* SysTick end of count event each 10ms */
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

    while (1)
    {
        printf("Hi Mom\n");
        Delay(20);
    }
}

/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds.
 * @retval None
 */
void Delay(__IO uint32_t nTime)
{
    uwTimingDelay = nTime;
    while(uwTimingDelay != 0);
}

/**
 * @brief  Decrements the TimingDelay variable.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    if (uwTimingDelay != 0x00)
    {
        uwTimingDelay--;
    }
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}

#endif
