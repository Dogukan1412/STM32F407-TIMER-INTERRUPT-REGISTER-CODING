
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void Led_Config(void);
void CLK_Config(void);
void TIM4_Config(void);
void Delay(uint32_t);

uint16_t count = 0;

int main(void)
{
	Led_Config();
	CLK_Config();
	TIM4_Config();
  while (1)
  {
	  GPIOD->ODR = 0x00000000;   // pins reset
	  count = TIM4->CNT;			// this code was written for us to see the counter value from the STM Studio
  }
}


void CLK_Config() // Clock speed for 168MHz
{
	RCC->CR |= 0x00010000;                 // HSEON ENABLE
	while(!(RCC->CR & 0x00020000));        // HSEON Ready Flag wait
	RCC->CR |= 0x00080000;              // CSS ENABLE
	RCC->CR |= 0x01000000;				// PLL ON
	RCC->PLLCFGR |= 0x00400000;        // PLL SRC HSE is selected
	RCC->PLLCFGR |= 0x00000004;       // PLL M 4
	RCC->PLLCFGR |= 0x00005A00;        // PLL N 168
	RCC->PLLCFGR |= 0x00000000;       // PLL P 2
	RCC->CFGR |= 0x00000000;          // AHB PRESCALER 1
	RCC->CFGR |= 0x00080000;          // APB2 PRESCALER 2
	RCC->CFGR |= 0x00001400;          // APB1 PRESCALER 4
	RCC->CIR |= 0x00080000;             // HSE READY FLAG CLEAR
	RCC->CIR |= 0x00800000;             // CSS FLAG CLEAR
}


void Led_Config(void)  // User led configuration
{
	RCC->AHB1ENR |= 0x1U << 3U; // D port clock enable

	GPIOD->MODER |= 0x55000000; // pins D12, D13, D14, D15 is selected output mode
	GPIOD->OSPEEDR |= 0xFF000000; // very high speed is selected
	GPIOD->PUPDR |= 0x00000000; // no pull up, pull down
}


void TIM4_Config(void)
{
	RCC->APB1ENR |= 0x1U << 2U;				// timer4 clock bus enabled

	TIM4->CR1 |= 0x1U << 0U;				// counter enable
	TIM4->CR1 |= 0x0U << 4U;				// count up
	TIM4->CR1 |= 0x0U << 5U;				// edge aligned mod
	TIM4->CR1 |= 0x1U << 7U;				// TIM4_ARR register is buffered
	TIM4->CR1 |= 0x0U << 8U;				// clock divided 1
	TIM4->SMCR |= 0x0U << 0U;				// slave mode disable
	TIM4->PSC = 8399;						// prescaler = 8399
	TIM4->ARR = 49999;						// period = 49999
	//  84 000 000 / ( (49999 + 1) * (8399 + 1) ) = 0.2 Hertz = 5 second

	TIM4->CR1 |= 0x1U << 2U;				// update request source
	TIM4->CR1 |= 0x0U << 1U;				// update event enable
	TIM4->DIER |= 0x1U << 0U;				// update interrupt enabled

	NVIC_EnableIRQ(TIM4_IRQn);				// Enable nested vector interrupt


}


void TIM4_IRQHandler(void)
{
	 GPIOD->ODR = 0x0000F000;  						 // pins set
	 Delay(20000000);          						 // wait for 1 second
	 TIM4->SR &= ~(0x1U << 0x0U);					 //update interrupt flag is cleared
}


void Delay(uint32_t time)
{
	while(time--);
}



/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
