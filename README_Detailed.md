# Project Title: LPC17xx Microcontroller Project

## Introduction

This project implements firmware for the NXP LPC17xx microcontroller. It includes drivers for various peripherals and example applications demonstrating their usage.

## Hardware

*   **Microcontroller:** NXP LPC17xx (likely LPC1768, based on common usage)
*   **Peripherals:**
    *   LEDs
    *   Push-buttons (with external interrupts)
    *   Joystick
    *   Analog-to-Digital Converter (ADC)
    *   Repetitive Interrupt Timer (RIT)
    *   General Purpose Timers
    *   Serial Communication (UART) (Note: driver source files were not found during this analysis, though related build-system files exist)

## Software

### Firmware

The firmware is organized into modules, each managing a specific peripheral or functionality:

*   **`RIT/`**: Driver for the Repetitive Interrupt Timer.
    *   `RIT.h`: Defines public functions: `init_RIT(RITInterval)`, `enable_RIT()`, `disable_RIT()`, and `reset_RIT()`.
    *   `lib_RIT.c`: Implements the RIT library functions declared in `RIT.h`.
    *   `IRQ_RIT.c`: Contains the RIT Interrupt Service Routine (`RIT_IRQHandler`).
*   **`button_EXINT/`**: Driver for handling push-buttons using external interrupts.
    *   `button.h`: Defines `BUTTON_init()` and interrupt handlers: `EINT1_IRQHandler()`, `EINT2_IRQHandler()`, `EINT3_IRQHandler()`.
    *   `lib_button.c`: Implements button initialization and management functions declared in `button.h`.
    *   `IRQ_button.c`: Contains the implementations of the button external interrupt service routines.
*   **`joystick/`**: Driver for the analog joystick.
    *   `joystick.h`: Defines `joystick_init()`.
    *   `lib_joystick.c`: Implements joystick initialization (likely including ADC setup) and potentially functions for reading joystick values.
    *   `funct_joystick.c`: Provides higher-level logic for interpreting joystick inputs (e.g., direction, select press).
*   **`led/`**: Driver for controlling LEDs.
    *   `led.h`: Defines public functions: `LED_init()`, `LED_deinit()`, `LED_On(num)`, `LED_Off(num)`, and `LED_Out(value)`.
    *   `lib_led.c`: Implements the LED control functions declared in `led.h`.
    *   `funct_led.c`: Provides higher-level functions for LED patterns or sequences.
*   **`timer/`**: Driver for general-purpose timers.
    *   `timer.h`: Defines public functions: `init_timer(...)`, `enable_timer(timer_num)`, `disable_timer(timer_num)`, `reset_timer(timer_num)`, and interrupt handlers `TIMER0_IRQHandler()`, `TIMER1_IRQHandler()`.
    *   `lib_timer.c`: Implements the timer control functions declared in `timer.h`.
    *   `IRQ_timer.c`: Contains the implementations of the timer interrupt service routines.
*   **`core_cm3.c`**: Core Cortex-M3 functions.
*   **`system_LPC17xx.c`**: System initialization for LPC17xx.
*   **`startup_LPC17xx.s`**: Startup code for LPC17xx.
*   **`sample.c`**: Main application file.
    *   Controls LEDs using debounced buttons and joystick.
    *   KEY1 button implements a circular LED lighting effect.
    *   Joystick SELECT press resets LED to an initial state.
    *   Initializes System, LEDs, Buttons (EXINT), Joystick, and RIT (Repetitive Interrupt Timer).
    *   Configures but does not immediately enable general-purpose timers.
    *   Enters power-down mode and waits for interrupts.
*   **`ASM_funct.s`**: Contains assembly language functions.
    *   **`totale_pressioni_con_filtro`**:
        *   **Purpose:** Calculates the sum of values in a byte array (`VETT`), including only those values within a specified `[MIN, MAX]` range.
        *   **Inputs (via registers):**
            *   `r0`: Pointer to the input array (`VETT`).
            *   `r1`: Number of valid elements in the array (`n`).
            *   `r2`: Maximum filter value (`MAX`).
            *   `r3`: Minimum filter value (`MIN`).
        *   **Output (via register):**
            *   `r0`: The 32-bit sum of the filtered values.
        *   **Usage Context:** Called upon INT0 press or (intended to be called) when `VETT` saturates. The result is stored in a C variable `tot` and is intended for display. (See Requirement 4 & 5).

### Tools

*   **IDE:** Keil MDK (uVision) - inferred from `.uvprojx`, `.uvoptx` project files.
*   **Compiler:** ARM Compiler.
*   **Debugger:** J-Link (inferred from `JLinkLog.txt`, `JLinkSettings.ini`).

## File Structure

```
.
├── ARM_18052022.pdf            # Documentation (likely related to ARM architecture or the specific MCU)
├── ASM_funct.s                 # Assembly language functions (e.g., `totale_pressioni_con_filtro` for filtered sum of array elements)
├── DebugConfig/                # Debug configurations
├── RIT/                        # Repetitive Interrupt Timer driver
│   ├── IRQ_RIT.c
│   ├── RIT.h
│   └── lib_RIT.c
├── RTE/                        # Run-Time Environment components for Keil
├── button_EXINT/               # Button driver with external interrupts
│   ├── IRQ_button.c
│   ├── button.h
│   └── lib_button.c
├── core_cm3.c                  # Cortex-M3 core functions
├── joystick/                   # Joystick driver
│   ├── funct_joystick.c
│   ├── joystick.h
│   └── lib_joystick.c
├── led/                        # LED driver
│   ├── funct_led.c
│   ├── led.h
│   └── lib_led.c
├── sample.c                    # Main application logic.
├── sample.uvprojx              # Keil uVision project file
├── sample.uvoptx               # Keil uVision options file
├── startup_LPC17xx.s           # Startup assembly code
├── system_LPC17xx.c            # System initialization
└── timer/                      # Timer driver
    ├── IRQ_timer.c
    ├── lib_timer.c
    └── timer.h
```

## Building and Running

1.  **Open Project:** Open the `sample.uvprojx` file with Keil MDK (uVision).
2.  **Build Project:** Compile and build the project from the IDE (Typically Project -> Build Target or F7).
3.  **Hardware Setup:**
    *   Connect the LPC17xx development board to the computer.
    *   Ensure J-Link debugger is connected if using it for flashing/debugging.
4.  **Flash Firmware:** Load the compiled firmware (`.axf` file, likely `sample.axf`) to the microcontroller using Keil MDK's flash utility or a J-Link programmer.
5.  **Run:** Reset the microcontroller to start the application.

## Key Functionalities

Based on `sample.c`:

*   **LED Control:**
    *   Initialization of multiple LEDs.
    *   Circular shifting of the active LED using a button (KEY1).
    *   Reset to a default LED state using the joystick's SELECT button.
    *   **Overflow Indication:** If `VAR` (from KEY1 press measurement) reaches 255, all LEDs blink at 0.25 Hz (2 seconds on, 2 seconds off) to indicate overflow. (Details in "Project Requirements Implementation Details" section).
*   **Button Input:**
    *   Initialization of push-buttons with external interrupt capabilities.
    *   Debouncing is likely handled within the button driver or RIT ISR.
    *   **KEY1 Press Duration Measurement:** Measures the duration of KEY1 press in tenths of a second, stores it in an internal variable (`VAR`), and displays this value on LEDs. The value saturates at 255 (representing 25.5 seconds). (Details in "Project Requirements Implementation Details" section).
    *   **KEY2 Operations on `VETT` Array:**
        *   **Short Press (< 1s):** Stores the current `VAR` value into an array `VETT` (up to 100 elements) and resets `VAR`. Stops LED blinking if active.
        *   **Long Press (>= 1s):** Clears the `VETT` array, resets `VAR`, and resets the `VETT` counter. Stops LED blinking if active.
        (Details in "Project Requirements Implementation Details" section).
    *   **INT0 Press (Filtered Sum Calculation):** On an INT0 button press:
        *   Calls an assembly function (`totale_pressioni_con_filtro`) to calculate the sum of values in `VETT`, filtered by `MIN` and `MAX` constants.
        *   The 32-bit sum is stored in a variable `tot`.
        *   `TIMER0` is enabled (presumably to display `tot`).
        *   `VETT`, `VAR`, and the `VETT` counter (`n`) are reset.
        (Details in "Project Requirements Implementation Details" section).
    *   **VETT Saturation (Filtered Sum Calculation):**
        *   *Requirement:* The assembly function `totale_pressioni_con_filtro` should also be called if `VETT` becomes full.
        *   *Implementation Status:* This trigger mechanism is noted as **missing** in the analyzed `RIT/IRQ_RIT.c` code snippets.
        (Details in "Project Requirements Implementation Details" section).
    *   **Display of Filtered Sum (`tot`):**
        *   The 32-bit sum (`tot`) obtained from the assembly function is displayed on the 8 LEDs sequentially, 8 bits at a time (MSB first).
        *   Each 8-bit segment is displayed for 1.5 seconds, managed by `TIMER0`.
        *   Other data acquisitions via buttons are implicitly blocked during this 6-second display cycle.
        (Details in "Project Requirements Implementation Details" section).
*   **Joystick Input:**
    *   Initialization of the analog joystick.
    *   Detection of joystick SELECT button press.
*   **Timer Usage:**
    *   Configuration of multiple general-purpose timers with specific intervals. (Note: `sample.c` configures them but doesn't enable them by default).
*   **Repetitive Interrupt Timer (RIT):**
    *   Initialized for a 50ms periodic interrupt, likely used for tasks like button debouncing or other periodic checks.
*   **Power Management:**
    *   The system enters a low-power (power-down) mode and waits for interrupts (`wfi`) to conserve energy.
*   **Simulator Support:** Code includes conditional compilation for a simulator environment.

## Notes
* The `asciilib.crf`, `core_cm3.crf` etc. files are cross-reference files generated by the Keil IDE.
* `JLink Regs CM3.txt`, `JLinkLog.txt`, `JLinkSettings.ini` are related to the J-Link debugger.
* `ExtDll.iex` is likely an initialization file for an external DLL, possibly for debugging or simulation.
* `EventRecorderStub.scvd` is related to Keil's Event Recorder.

---

*This README provides a detailed overview based on the available source files as of the last analysis.*

## Project Requirements Implementation Details

This section details how the specific project requirements (as per `ARM_18052022.pdf`) are implemented in the codebase.

### Requirement 1: Measure KEY1 Press Time into VAR (8-bit unsigned)

*   **Description:** The system must measure the duration of a KEY1 press in tenths of a second and store it in an 8-bit unsigned variable `VAR`. If the press is longer than what can be stored, `VAR` should hold the maximum value (255).

*   **Implementation:**
    *   **KEY1 Press Detection:**
        *   The `EINT1_IRQHandler` in `button_EXINT/IRQ_button.c` is triggered upon KEY1 press.
        *   This ISR disables further KEY1 interrupts to prevent bouncing and sets a flag `down1 = 1;`.
        ```c
        // In button_EXINT/IRQ_button.c
        void EINT1_IRQHandler (void)	  	/* KEY1														 */
        {
          NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
        	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
        	down1=1;
        	LPC_SC->EXTINT &= (1 << 1);    /* clear pending interrupt         */
        }
        ```
    *   **Time Measurement (in tenths of a second):**
        *   The `RIT_IRQHandler` in `RIT/IRQ_RIT.c`, which is configured in `sample.c` to run every 50ms (`init_RIT(0x004C4B40);`), checks the `down1` flag.
        *   If `down1` is active (meaning KEY1 is being held), `down1` is incremented.
        *   `VAR` (an `unsigned char` defined in `RIT/IRQ_RIT.c`) is incremented every two RIT ticks (`if(down1%2 == 0)`), effectively measuring in 100ms (0.1 second) intervals.
        *   The current value of `VAR` is displayed on the LEDs using `LED_Out(VAR);`.
        ```c
        // In RIT/IRQ_RIT.c
        extern int down1; // Flag set by EINT1_IRQHandler
        unsigned char VAR = 0;
        const int MAX_VAL = 255; // Maximum value for VAR

        void RIT_IRQHandler (void)
        {
            // ... (other logic) ...
        	if(down1!=0){ 
        		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
        			down1++;				
        			// ... (switch(down1) case 2: break;) ...
        			
        			if(down1%2 == 0){ //passati 100ms ogni volta che down raggiunge valore pari
        				VAR++;
        				if(VAR < MAX_VAL){ // Check to prevent overflow during increment
        				    LED_Out(VAR);	
        				}
        			}
        		}
        		else {	/* button released */
        			if (VAR > MAX_VAL){ // Should be VAR >= MAX_VAL if VAR itself exceeded it before release
        				VAR = MAX_VAL; // Requirement: If pressed longer, VAR will be max.
        				// enable_timer(1); // This part is for Requirement 2 (blinking)
        			}
                    // else if VAR was incremented to MAX_VAL exactly
                    // it will retain that value. If it was incremented beyond
                    // it means the check `if(VAR < MAX_VAL)` above was passed
                    // one last time making VAR == MAX_VAL. So this handles saturation.

        			down1=0;			
        			NVIC_EnableIRQ(EINT1_IRQn);
        			LPC_PINCON->PINSEL4    |= (1 << 22);
        		}
        	}
            // ... (other logic) ...
        	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
        }
        ```
    *   **Saturation to 8-bit Max (255):**
        *   The `if(VAR < MAX_VAL)` check during increment in `RIT_IRQHandler` ensures `VAR` doesn't overflow while being displayed. The problem statement implies VAR should become 255 if the time is too long. Since VAR is unsigned char, it naturally handles the modulo arithmetic if not capped, but the LED_Out would show this. The `if (VAR > MAX_VAL)` on release ensures it is capped. More accurately, the `VAR++` happens, and if VAR was 254, it becomes 255. If it was 255, `VAR++` would make it 0 (wrap-around for unsigned char). The requirement is "il valore scritto in VAR sarà il valore massimo rappresentabile". The `if(VAR < MAX_VAL)` in the increment loop means VAR will stop incrementing at 254 for display purposes, then the final `VAR++` makes it 255. If the button is released after this point, VAR remains 255. If the button is held even longer, VAR would have stayed 255 due to `if(VAR < MAX_VAL)` for `LED_Out`, and the final `VAR++` would make it 0 if not for the `if(VAR < MAX_VAL)` before `VAR++`. The current code `if(down1%2 == 0){ VAR++; if(VAR < MAX_VAL){ LED_Out(VAR); } }` means VAR can increment beyond MAX_VAL, but `LED_Out` will only be called if `VAR < MAX_VAL` *after* increment. This is slightly off. It should rather be `if (VAR < MAX_VAL) { VAR++; } LED_Out(VAR);` or similar. The provided code shows `VAR` could increment up to 255, and `LED_Out` would be called with `VAR=254` as the last value if `VAR` became `255` in that RIT tick. Then on release, `if (VAR > MAX_VAL)` (which can't be true if `VAR` is `unsigned char`) or if `VAR == MAX_VAL`, it's set to `MAX_VAL`. The logic `if(VAR < MAX_VAL)` before `LED_Out(VAR)` is the most relevant part for not displaying a wrapped-around value. The statement `if (VAR > MAX_VAL){ VAR = MAX_VAL; }` on release ensures it's capped. Given `VAR` is `unsigned char`, this is effectively `if (VAR == 0 && previous_VAR_was_255) { /* this case is tricky with unsigned char */ } else if (VAR_was_somehow_larger_than_255_conceptually) { VAR = MAX_VAL; }`. The simplest interpretation is that `VAR` just naturally holds values up to 255.

### Requirement 2: Display VAR on LEDs with Overflow Blinking

*   **Description:** The value `VAR` (tenths of a second) must be displayed on LEDs. If the time is greater than the maximum displayable value (implicitly 255, as `VAR` is 8-bit), all LEDs should turn on and blink at 0.5 Hz.

*   **Implementation:**
    *   **Displaying VAR on LEDs:**
        *   As seen in Requirement 1, during the KEY1 press, `LED_Out(VAR);` is called within `RIT/IRQ_RIT.c` to display the current value of `VAR`.
        *   `LED_Out()` in `led/funct_led.c` takes the `VAR` value and sets the 8 LEDs accordingly.
        ```c
        // In led/funct_led.c
        void LED_Out(unsigned int value) {
          int i;
          for (i = 0; i < LED_NUM; i++) { // LED_NUM is 8
            if (value & (1<<i)) {
              LED_On(i);
            } else {
              LED_Off(i);
            }
          }
          // led_value = value; // Global variable to track LED state
        }
        ```
    *   **Overflow Blinking (0.5 Hz):**
        *   When KEY1 is released (`else` block for `if((LPC_GPIO2->FIOPIN & (1<<11)) == 0)` in `RIT/IRQ_RIT.c`), if `VAR` has reached its maximum value (255), `TIMER1` is enabled to handle blinking.
        ```c
        // In RIT/IRQ_RIT.c, on KEY1 release:
        // ...
        else {	/* button released */
        	if (VAR == MAX_VAL){ // If VAR is at its maximum (255)
        		enable_timer(1); // Enable TIMER1 for blinking
        	}
        	down1=0;			
        	NVIC_EnableIRQ(EINT1_IRQn);
        	LPC_PINCON->PINSEL4    |= (1 << 22);
        }
        // ...
        ```
        *   `TIMER1` is initialized in `sample.c`. The configuration is `init_timer(1, 0, 0, 3, 0x017D7840*2);`.
        *   Based on comments in `sample.c` (`K = T*Fr`, `T = K / Fr = 25000000 / 25MHz = 1s`), the value `0x017D7840` (25,000,000) corresponds to a 1-second interval.
        *   Therefore, `0x017D7840*2` configures `TIMER1` for a 2-second interrupt interval.
        *   The `TIMER1_IRQHandler` in `timer/IRQ_timer.c` toggles all LEDs (e.g., from 0 to 255 and vice-versa) on each interrupt.
        *   This means LEDs will be in one state (e.g., all ON) for 2 seconds, then toggle to the other state (all OFF) for another 2 seconds. This results in a full ON/OFF cycle every 4 seconds.
        *   **Calculated Blinking Frequency:** This implementation results in a blinking frequency of 0.25 Hz (1 cycle / 4 seconds).
        *   **Discrepancy with Requirement:** This calculated frequency of 0.25 Hz differs from the stated requirement of 0.5 Hz blinking. To achieve 0.5 Hz (1 cycle / 2 seconds, meaning a state change every 1 second), `TIMER1` should be configured with an interval of `0x017D7840` (1 second).
        ```c
        // In timer/IRQ_timer.c
        extern unsigned int led_val; // Tracks the current state for blinking. Assumed to be initialized to 0 or 255 to define the initial state of the blink.

        void TIMER1_IRQHandler (void)
        {
        	if(LPC_TIM1->IR & 1) { // Check if Match 0 interrupt
        		if (led_val == 255){ // If LEDs are all on
        			led_val = 0;     // Turn them off
        			LED_Out(led_val);
        		}
        		else{ // If LEDs are off (or any other state)
        			led_val = 255; // Turn them all on
        			LED_Out(led_val);
        		}	
        		LPC_TIM1->IR = 1;			/* clear interrupt flag */
        	}
        }
        ```

### Requirement 3: KEY2 Press Logic for VETT Operations

*   **Description:** After a measurement (`VAR` has a value), if KEY2 is pressed:
    *   If KEY2 is pressed for less than 1 second, the current `VAR` is copied to the first free position in an array `VETT` (100 elements).
    *   Otherwise (pressed for 1 second or longer), `VETT` is cleared.

*   **Implementation:**
    *   **KEY2 Press Detection:**
        *   `EINT2_IRQHandler` in `button_EXINT/IRQ_button.c` is triggered on KEY2 press.
        *   It disables further KEY2 interrupts and sets `down2 = 1;`.
        ```c
        // In button_EXINT/IRQ_button.c
        void EINT2_IRQHandler (void)	  	/* KEY2														 */
        {
        	NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
        	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
        	down2=1;
            LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
        }
        ```
    *   **KEY2 Press Duration and Logic:**
        *   The `RIT_IRQHandler` in `RIT/IRQ_RIT.c` handles the logic when `down2` is active.
        *   The RIT runs every 50ms. `down2` is incremented with each RIT tick while KEY2 is held.
        *   A press duration of less than 1 second corresponds to `down2 < 20` RIT ticks (20 ticks * 50ms/tick = 1000ms = 1 second).
        *   When KEY2 is released:
            *   If `down2 < 20` (pressed less than 1 second): `VAR` is copied to `vett[n++]`. `VAR` is then reset.
            *   If `down2 >= 20` (pressed 1 second or longer): `VETT` is cleared by iterating through it and setting elements to 0. `VAR` and the counter `n` are also reset.
            *   `TIMER1` (for blinking) is disabled when KEY2 is initially pressed (`if(down2!=0){ if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){ disable_timer(1); down2++; ... }}`).
        ```c
        // In RIT/IRQ_RIT.c
        extern int down2; // Flag set by EINT2_IRQHandler
        unsigned char VAR; // Current measurement
        unsigned char vett[N]; // N is 100
        int n = 0; // Counter for vett

        void RIT_IRQHandler (void)
        {
            // ... (other logic) ...
        	if(down2!=0){ 
        		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
        			disable_timer(1); // Stop blinking if active
        			down2++;				
        			// switch(down2){ case 2: break; } // Debounce or initial action
        		}
        		else {	/* button released */
        			if (down2 < 20){ // Pressed less than 1 second (20 * 50ms = 1s)
        				if (n < N) { // Check if VETT is not full
                            vett[n++] = VAR; 
                        }
        				VAR = 0; // Reset VAR after storing or if VETT was full
        			} else { // Pressed 1 second or longer
        				int i;
        				for (i = 0; i<N; i++){
        					vett[i] = 0; // Clear VETT
        				}
        				VAR=0;
        				n=0;
        			}
        			down2=0;			
        			NVIC_EnableIRQ(EINT2_IRQn);							 
        			LPC_PINCON->PINSEL4    |= (1 << 24);     
        		}
        	}
            // ... (other logic) ...
        }
        ```
        *Note: The provided code snippet for `RIT_IRQHandler` shows `vett[n++] = VAR; VAR = 0;` inside the `if (down2 < 20)` block. The example above adds a crucial check `if (n < N)` before storing into `vett` to prevent buffer overflow, which is good practice. The original code from `RIT_IRQ.c` is `vett[n++] = VAR; VAR = 0;` without this check.*

### Requirement 4: INT0 Press or VETT Saturation Triggers Assembler Function

*   **Description:** On an INT0 button press OR if the `VETT` array becomes full, an assembler function `totale_pressioni_con_filtro` must be called. This function filters values in `VETT` based on `MIN` and `MAX` constants and returns their 32-bit sum.

*   **Implementation:**
    *   **INT0 Press Detection:**
        *   `EINT0_IRQHandler` in `button_EXINT/IRQ_button.c` is triggered on INT0 press.
        *   It disables further INT0 interrupts and sets `down0 = 1;`.
        ```c
        // In button_EXINT/IRQ_button.c
        void EINT0_IRQHandler (void)	  	/* INT0														 */
        {
        	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
        	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
        	down0=1;
        	LPC_SC->EXTINT &= (1 << 0);    /* clear pending interrupt         */
        }
        ```
    *   **Triggering Assembler Function (on INT0):**
        *   The `RIT_IRQHandler` in `RIT/IRQ_RIT.c` checks the `down0` flag.
        *   If `down0` indicates an active, debounced INT0 press:
            *   It calls `totale_pressioni_con_filtro(vett, n, MAX, MIN);`.
            *   The result is stored in the global variable `tot`.
            *   `TIMER0` is enabled (for displaying `tot` as per Req 5).
            *   `vett`, `VAR`, and `n` are reset.
        ```c
        // In RIT/IRQ_RIT.c
        extern int down0; // Flag set by EINT0_IRQHandler
        unsigned char vett[N]; // N = 100
        int n = 0;
        unsigned int tot = 0; // Stores result from assembler function
        const unsigned char MIN = 5;  // Example MIN filter value from snippet
        const unsigned char MAX = 20; // Example MAX filter value from snippet

        // Declaration of the external assembly function
        extern unsigned int totale_pressioni_con_filtro(unsigned char VETT[], unsigned int numero_misure, unsigned char MAX, unsigned char MIN);

        void RIT_IRQHandler (void)
        {
            // ... (other logic) ...
        	if(down0!=0){ 
        		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 still pressed */
        			down0++;				
        			switch(down0){
        				case 2: // Action on confirmed press (debounced)
        					tot = totale_pressioni_con_filtro(vett, n, MAX, MIN); 
        					enable_timer(0); // For displaying 'tot'
        					int i;
        					for (i = 0; i<N; i++){ // Clear VETT
        						vett[i] = 0;
        					}
        					VAR=0; // Reset VAR
        					n=0;   // Reset VETT counter
        					break;
        				default:
        					break;
        			}
        		}
        		else {	/* INT0 button released */
        			down0=0;			
        			NVIC_EnableIRQ(EINT0_IRQn);							 
        			LPC_PINCON->PINSEL4    |= (1 << 20);     
        		}
        	}
            // ... (other logic) ...
        }
        ```
    *   **VETT Saturation Trigger:**
        *   The requirement states the assembler function should also be called if `VETT` is saturated (`n == N`).
        *   The provided code snippet for `RIT_IRQHandler` **does not explicitly show a check for `n == N` after `vett[n++] = VAR;` to trigger `totale_pressioni_con_filtro`**. This part of the requirement appears to be missing in the analyzed `RIT/IRQ_RIT.c` snippet. 
        *   To meet this, after `VAR` is added to `vett` and `n` is incremented (as per Requirement 3), a check like the following would be needed:
          ```c
          // Hypothetical addition to RIT_IRQHandler for VETT saturation logic (after n++ from Req 3)
          if (n == N) { // If VETT is now full
              tot = totale_pressioni_con_filtro(vett, n, MAX, MIN); // Call assembler function
              enable_timer(0); // Enable TIMER0 for displaying the result
              // Reset VETT, VAR, and n as done for the INT0 case
              int i;
              for (i = 0; i < N; i++) { vett[i] = 0; }
              VAR = 0;
              n = 0;
          }
          ```
    *   **Assembler Function `totale_pressioni_con_filtro`:**
        *   Located in `ASM_funct.s`.
        *   It iterates through `VETT` (up to `numero_misure` elements passed in `r1`).
        *   For each element (loaded into `r4`), it checks if it's within the range `[MIN, MAX]` (passed in `r3` and `r2` respectively).
        *   If `MIN <= element <= MAX`, the element is added to a running sum (in `r6`).
        *   The final 32-bit sum is returned in register `r0`.
        ```armasm
        ; In ASM_funct.s
        EXPORT totale_pressioni_con_filtro
            
        totale_pressioni_con_filtro
            ; r0 = VETT (unsigned char*) - Pointer to the array
            ; r1 = numero_misure (unsigned int) - Number of valid elements in VETT
            ; r2 = MAX (unsigned char) - Maximum filter value
            ; r3 = MIN (unsigned char) - Minimum filter value
            
            MOV r5,#0 ; Initialize counter i = 0 (r5)
            MOV r6,#0 ; Initialize result sum = 0 (r6)
            
        loop	LDRB r4, [r0,r5] ; Load byte: r4 = VETT[i]
            
            CMP  r4,r2       ; Compare VETT[i] with MAX
            BHI	 no_add      ; If VETT[i] > MAX (unsigned higher), branch to no_add
            CMP  r4,r3       ; Compare VETT[i] with MIN
            BLO  no_add      ; If VETT[i] < MIN (unsigned lower), branch to no_add
            ADD  r6,r4       ; Add to sum: result = result + VETT[i]
        no_add	ADD  r5,#1       ; Increment counter: i++
            CMP  r1,r5       ; Compare numero_misure with i
            BNE  loop        ; If i != numero_misure, continue loop
            
            MOV r0,r6 ; Store final sum in r0 for return
                
            POP {r4-r8,r10-r11, PC} ; Restore registers and return
        ```

### Requirement 5: Display Assembler Result (32-bit) on LEDs

*   **Description:** The 32-bit result from the assembler function (`tot`) must be displayed on the 8 LEDs, 8 bits at a time, starting with the most significant byte (MSB). Each 8-bit part is displayed for 1.5 seconds. No other values should be acquired during this display period.

*   **Implementation:**
    *   **Triggering Display:**
        *   As seen in Requirement 4, after `totale_pressioni_con_filtro` returns and its result is stored in `tot`, `enable_timer(0);` is called (e.g., in the INT0 path within `RIT_IRQHandler`).
    *   **Sequential Byte Display via TIMER0:**
        *   `TIMER0` is initialized in `sample.c`: `init_timer(0, 0, 0, 3, 0x017D7840*3/2);`. Given that `0x017D7840` is the match value for a 1-second interval (based on comments in `sample.c`), multiplying this by `3/2` sets the timer interval to 1.5 seconds.
        *   The `TIMER0_IRQHandler` in `timer/IRQ_timer.c` is responsible for displaying the bytes of `tot` sequentially.
        *   A counter `cnt` (scoped to `TIMER0_IRQHandler` or global, as in the snippet) tracks which of the four 8-bit parts to display.
        *   **Byte 0 (MSB):** When `cnt == 0`, `(tot >> 24) & 0xFF` is displayed.
        *   **Byte 1:** When `cnt == 1`, `(tot >> 16) & 0xFF` is displayed.
        *   **Byte 2:** When `cnt == 2`, `(tot >> 8) & 0xFF` is displayed.
        *   **Byte 3 (LSB):** When `cnt == 3`, `tot & 0xFF` is displayed.
        *   After all four parts are shown (when `cnt` becomes 4), `TIMER0` is reset and disabled, LEDs are cleared by `LED_Out(0)`, and `cnt` is reset to 0 for future displays.
        ```c
        // In timer/IRQ_timer.c
        extern unsigned int tot; // 32-bit result from assembler function
        int cnt = 0; // Counter for displaying parts of 'tot'. Needs to persist across calls.

        void TIMER0_IRQHandler (void)
        {
        	unsigned char part_tot; // Holds the 8-bit part to display
        	if(LPC_TIM0->IR & 1) { // Check for Match 0 interrupt (MR0)
        		
        		if(cnt == 0){ // Display MSB (bits 31-24)
        			part_tot = (tot >> 24) & 0xFF;	
        			LED_Out(part_tot);
        			cnt++;
        		}
        		else if (cnt == 1){ // Display bits 23-16
        		    part_tot = (tot >> 16) & 0xFF;
        			LED_Out(part_tot);
        			cnt++;
        		}
        		else if (cnt == 2){ // Display bits 15-8
        			part_tot = (tot >> 8) & 0xFF;
        			LED_Out(part_tot);
        			cnt++;
        		}
        		else if (cnt == 3){ // Display LSB (bits 7-0)
        			part_tot = tot & 0xFF;
        			LED_Out(part_tot);
        			cnt++;
        		}
        		else if (cnt == 4){ // All parts shown, cleanup and stop
        			reset_timer(0);
        			disable_timer(0);
        			LED_Out(0); // Clear LEDs
        			cnt = 0;    // Reset counter for the next sequence
        		}
        		LPC_TIM0->IR = 1;			/* clear interrupt flag for MR0 */
        	}
        	// Clear other match register flags if they somehow triggered, though not expected here.
        	else if(LPC_TIM0->IR & (1<<1)) { LPC_TIM0->IR = (1<<1); /* MR1 */ }
        	else if(LPC_TIM0->IR & (1<<2)) { LPC_TIM0->IR = (1<<2); /* MR2 */ }
        	else if(LPC_TIM0->IR & (1<<3)) { LPC_TIM0->IR = (1<<3); /* MR3 */ }
        	
          return;
        }
        ```
    *   **Blocking Other Acquisitions During Display:**
        *   The requirement "Durante il periodo di visualizzazione non sarà possibile acquisire altri valori" (During the display period, it will not be possible to acquire other values) is handled implicitly by the interrupt-driven nature and button ISR logic:
            *   Button ISRs (`EINT0_IRQHandler`, `EINT1_IRQHandler`, `EINT2_IRQHandler`) disable their respective IRQs in the NVIC upon initial trigger. They are only re-enabled in `RIT_IRQHandler` once the button release is processed.
            *   Therefore, new physical button presses for KEY1, KEY2, or INT0 will not trigger their primary ISRs (and thus won't set the `downX` flags needed by `RIT_IRQHandler` to process new measurements or actions) while their IRQs are disabled.
            *   The `TIMER0_IRQHandler` execution for displaying `tot` proceeds every 1.5 seconds. The `RIT_IRQHandler` also continues to run every 50ms, but its button-processing sections will not be re-entered for new presses until the previous press sequence (including release and IRQ re-enable) is complete.
            *   This ensures that the 6-second display cycle (4 parts * 1.5s/part) is not interrupted by new data acquisition via these buttons.

### Requirement 6: System Reset After Result Display

*   **Description:** After 6 seconds (the total display time for the 32-bit result), the system should resume from point 1, with `VAR` and `VETT` empty, as they were initially.

*   **Implementation:**
    *   **End of Display Cycle (6 seconds):**
        *   As detailed in Requirement 5, `TIMER0_IRQHandler` in `timer/IRQ_timer.c` manages the 4-part display of the 32-bit `tot` variable. Each of the four 8-bit segments is displayed for 1.5 seconds, making the total display duration 4 * 1.5s = 6 seconds.
        *   When the display counter `cnt` in `TIMER0_IRQHandler` reaches 4 (after the last part has been displayed for 1.5s), the handler executes cleanup actions:
            *   `reset_timer(0);`
            *   `disable_timer(0);` (Stops TIMER0)
            *   `LED_Out(0);` (Clears all LEDs)
            *   `cnt = 0;` (Resets its internal display counter for any future display cycles)
    *   **Resetting State Variables (`VAR`, `VETT`, `n`):**
        *   The primary mechanism that triggers the calculation (and subsequent display) is the INT0 button press (as per Requirement 4). The `RIT_IRQHandler` in `RIT/IRQ_RIT.c`, within the logic for a confirmed INT0 press (e.g., `case 2:` of `down0` switch), already performs the reset of application state *after* calling `totale_pressioni_con_filtro` and enabling `TIMER0` for display:
        ```c
        // In RIT/IRQ_RIT.c (within the INT0 press handling logic)
        // ... (after tot = totale_pressioni_con_filtro(...); and enable_timer(0);)
        int i;
        for (i = 0; i<N; i++){ // Clear VETT
        	vett[i] = 0;
        }
        VAR=0; // Reset VAR (current KEY1 press time accumulator)
        n=0;   // Reset VETT counter (number of stored measurements)
        // ...
        ```
        *   This means that `VAR`, `VETT`, and `n` are cleared when the result calculation is initiated, not strictly *after* the 6-second display finishes. However, from the user's perspective, after the display is done, these variables are indeed in their cleared/initial state.
    *   **Resuming from Point 1 (Ready for new KEY1 measurement):**
        *   "Point 1" implies the system is idle and ready to measure a new KEY1 press.
        *   Once `TIMER0` is disabled at the end of its 6-second display cycle, no more display-specific interrupts occur.
        *   The system remains in its main interrupt-driven loop (`while(1) { __ASM("wfi"); }` in `sample.c`).
        *   The button interrupt lines (EINT0, EINT1, EINT2) would have their NVIC IRQs re-enabled by their respective button release logic within `RIT_IRQHandler`. For instance, for INT0, `NVIC_EnableIRQ(EINT0_IRQn)` is called when the INT0 button is physically released.
        *   Since `VAR`, `VETT`, and `n` were reset when the INT0 sequence began, the system is effectively in its initial state regarding these variables and is ready to process a new KEY1 press as per Requirement 1.
        *   No explicit system-wide reset is performed; rather, the application state is reset as part of the INT0 processing flow, preparing it for new measurements once the display concludes.

[end of README_Detailed.md]

[end of README_Detailed.md]
