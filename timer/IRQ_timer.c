/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "stdio.h"
#include "timer.h"
#include "string.h"
#include "../led/led.h"

#define N 100
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

/* defined in funct_led								*/

unsigned int led_val;
extern unsigned int tot;
int cnt = 0;

void TIMER0_IRQHandler (void)
{
	char part_tot;
	if(LPC_TIM0->IR & 1) {
	
		
		if(cnt == 0){
			part_tot = tot;	
			part_tot = part_tot >> 24;
			cnt++;
			LED_Out(part_tot);
			LPC_TIM0->IR = 1;			/* clear interrupt flag */
			return;
		}
		
		if (cnt == 1){
			part_tot = tot;	
		   part_tot = part_tot & 0x00FF0000;
			 part_tot = part_tot >> 16;
				cnt++;
			LED_Out(part_tot);
			LPC_TIM0->IR = 1;			/* clear interrupt flag */
			return;
		}
		
		if (cnt == 2){
			part_tot = tot;	
			part_tot = part_tot & 0x0000FF00;
			part_tot = part_tot >> 8;
			cnt++;
			LED_Out(part_tot);
			LPC_TIM0->IR = 1;			/* clear interrupt flag */
			return;
		}
		
		if (cnt == 3){
			part_tot = tot;	
			part_tot = part_tot & 0x000000FF;
			cnt ++;
			LED_Out(part_tot);
			LPC_TIM0->IR = 1;			/* clear interrupt flag */
			return;
		}
		
		if (cnt == 4){
			reset_timer(0);
			disable_timer(0);
			LED_Out(0);
		}
		
	}
		/* Match register 1 interrupt service routine */
	else if(LPC_TIM0->IR & 02)
  {
			
		LPC_TIM0->IR =  2 ;			/* clear interrupt flag */	
	}
	/* Match register 2 interrupt service routine */
	else if(LPC_TIM0->IR & 4)
  {
	 
		LPC_TIM0->IR =  4 ;			/* clear interrupt flag */	
	}
		/* Match register 3 interrupt service routine */
	else if(LPC_TIM0->IR & 8)
  {
	 
		LPC_TIM0->IR =  8 ;			/* clear interrupt flag */	
	}
	
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
 
	if(LPC_TIM1->IR & 1) {
		
		if (led_val == 255){
			led_val = 0;
			LED_Out(led_val);
		}
		else{
		
			led_val = 255;
			LED_Out(led_val);
		}	
		LPC_TIM1->IR = 1;			/* clear interrupt flag */
	}
}

void TIMER2_IRQHandler (void)
{
				
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
	
}


/******************************************************************************
**                            End Of File
******************************************************************************/
