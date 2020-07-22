/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */

#include "main.h"



typedef void @far (*interrupt_handler_t)(void);

@tiny uint16_t sys_count_ms;

struct interrupt_vector {
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

INTERRUPT void Timer2_it (void)
{
  sys_count_ms++;
	
	TIM2_ClearFlag(TIM2_FLAG_UPDATE);
  return;
}

INTERRUPT void Timer3_it (void)
{
	TIM3_ClearFlag(TIM3_FLAG_UPDATE);
	WDI_TOGGLE;
  return;
}
	

INTERRUPT void GPIOB_IT(void)
{
	BTN_it(&BTN_list[BTN_NAME_MOD]);
}

INTERRUPT void GPIOE_IT(void)
{
  BTN_it(&BTN_list[BTN_NAME_DOWN]);
}

INTERRUPT void GPIOA_IT(void)
{
  BTN_it(&BTN_list[BTN_NAME_UP]);
}

INTERRUPT void SPI_IT(void)
{
	spi_it();
}

INTERRUPT void ADC2_IT(void)
{
	adc_it();
}

@svlreg INTERRUPT void beCAN_RX_IT(void)
{
	can_arixprotocol_it();
}



@far @interrupt void NonHandledInterrupt (void)
{
	/* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
	return;
}

extern void _stext();     /* startup routine */

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext}, /* reset */
	{0x82, NonHandledInterrupt}, /* trap  */
	{0x82, NonHandledInterrupt}, /* irq0  */
	{0x82, NonHandledInterrupt}, /* irq1  */
	{0x82, NonHandledInterrupt}, /* irq2  */
	{0x82, GPIOA_IT}, /* irq3  */
	{0x82, GPIOB_IT}, /* irq4  */
	{0x82, NonHandledInterrupt}, /* irq5  */
	{0x82, NonHandledInterrupt}, /* irq6  */
	{0x82, GPIOE_IT}, /* irq7  */
	{0x82, beCAN_RX_IT}, /* irq8  */
	{0x82, NonHandledInterrupt}, /* irq9  */
	{0x82, SPI_IT}, /* irq10 */
	{0x82, NonHandledInterrupt}, /* irq11 */
	{0x82, NonHandledInterrupt}, /* irq12 */
	{0x82, Timer2_it}, /* irq13 */
	{0x82, NonHandledInterrupt}, /* irq14 */
	{0x82, Timer3_it}, /* irq15 */
	{0x82, NonHandledInterrupt}, /* irq16 */
	{0x82, NonHandledInterrupt}, /* irq17 */
	{0x82, NonHandledInterrupt}, /* irq18 */
	{0x82, NonHandledInterrupt}, /* irq19 */
	{0x82, NonHandledInterrupt}, /* irq20 */
	{0x82, NonHandledInterrupt}, /* irq21 */
	{0x82, ADC2_IT}, /* irq22 */
	{0x82, NonHandledInterrupt}, /* irq23 */
	{0x82, NonHandledInterrupt}, /* irq24 */
	{0x82, NonHandledInterrupt}, /* irq25 */
	{0x82, NonHandledInterrupt}, /* irq26 */
	{0x82, NonHandledInterrupt}, /* irq27 */
	{0x82, NonHandledInterrupt}, /* irq28 */
	{0x82, NonHandledInterrupt}, /* irq29 */
};


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

