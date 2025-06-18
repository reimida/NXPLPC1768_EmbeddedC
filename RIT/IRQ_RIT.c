/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../timer/timer.h"


#define N 100 

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern int down0;
extern int down1;
extern	int down2;

unsigned char VAR = 0;
unsigned char vett[N];
int n = 0;
int tot = 0;

const unsigned char MIN = 5;
const unsigned char MAX = 20; 
const int MAX_VAL = 255;

extern unsigned int totale_pressioni_con_filtro(unsigned char VETT[], unsigned int  numero_misure, unsigned char MAX, unsigned char MIN);


void RIT_IRQHandler (void)
{										
	static int select=0;
	static int J_down = 0;
	static int J_up = 0;
	static int position=0;	
		
	//////DOWN/////////////////
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		/* Joystick Select pressed p1.25*/
		/* Joystick Down pressed p1.26 --> using J_DOWN due to emulator issues*/
		/* Joystick Up pressed p1.29 */
    /* Joystick Left pressed 1.27 */
	  /* Joystick Right pressed 1.28 */			
		
		J_down++;
		switch(J_down){
			case 1:
				/*led_value++;
			  if(led_value>=255){
					led_value =0;
				}
				LED_Out(led_value);*/
				LED_Out(1);
				break;
			default:
				break;
		}
	}
	else{
			J_down=0;
	}
	
	/////////////UP//////////////////
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		/* Joystick Select pressed p1.25*/
		/* Joystick Down pressed p1.26 --> using J_DOWN due to emulator issues*/
		/* Joystick Up pressed p1.29 */
    /* Joystick Left pressed 1.27 */
	  /* Joystick Right pressed 1.28 */			
		
		J_up++;
		switch(J_up){
			case 1:
				/*led_value++;
			  if(led_value>=255){
					led_value =0;
				}
				LED_Out(led_value);*/
			  LED_Out(2);
				break;
			default:
				break;
		}
	}
	else{
			J_up=0;
	}

	
	/* LPC_GPIO2->FIOPIN & (1<<10)) == 0 --> INT0 pressed */
	/* LPC_GPIO2->FIOPIN & (1<<11)) == 0 --> KEY1 pressed */
	/* LPC_GPIO2->FIOPIN & (1<<12)) == 0 --> KEY2 pressed */
	/* button management */
	if(down0!=0){ 
		int i;
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* KEY0 pressed */
			down0++;				
			switch(down0){
				case 2:				/* pay attention here: please see slides to understand value 2 */
					tot = totale_pressioni_con_filtro(vett, n, MAX, MIN); 
					enable_timer(0);
				for (i = 0; i<N; i++){
					vett[i] = 0;
				}
				VAR=0;
				n=0;
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down0=0;			
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	
		if(down1!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
			down1++;				
			switch(down1){
				case 2:				/* pay attention here: please see slides to understand value 2 */
				//enable_timer(0);
					break;
				default:
					break;
			}
			
			if(down1%2 == 0){ //passati 100ms ogni volta che down raggiunge valore pari
				VAR++;
				if(VAR < MAX_VAL){
				LED_Out(VAR);	
				}
			}
		}
		else {	/* button released */
			if (VAR > MAX_VAL){
				VAR = MAX_VAL;
				enable_timer(1);
			}
			down1=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
		
	
		if(down2!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
			disable_timer(1);
			down2++;				
			switch(down2){
				case 2:				/* pay attention here: please see slides to understand value 2 */			
					break;
				default:
					break;
			}
			
		}
		else {	/* button released */
			if (down2 < 20){
				vett[n++] = VAR; 
				VAR = 0;
			}else {
				int i;
				for (i = 0; i<N; i++){
					vett[i] = 0;
				}
				VAR=0;
				n=0;
		}
			down2=0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}		
			


/******************************************************************************
**                            End Of File
******************************************************************************/
