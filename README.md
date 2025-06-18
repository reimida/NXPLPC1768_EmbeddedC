# Project Title: LPC17xx Microcontroller Project

## Introduction

This project implements firmware for the NXP LPC1768 microcontroller. It includes drivers for various peripherals and example applications demonstrating their usage.

## Hardware

*   **Microcontroller:** NXP LPC1768
*   **Peripherals:**
    *   LEDs
    *   Push-buttons (with external interrupts)
    *   Joystick
    *   Analog-to-Digital Converter (ADC)
    *   Repetitive Interrupt Timer (RIT)
    *   General Purpose Timers
    *   Serial Communication (UART)

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
*   **`startup_LPC17xx.s`**: Startup code for LPC17xx (defined memory, vector table and reset handler, which calls main)
*   **`sample.c`**: Main application file.
    *   Initializes System, LEDs, Buttons (EXINT), Joystick, and RIT (Repetitive Interrupt Timer).
    *   Configures but does not immediately enable general-purpose timers.
    *   Enters power-down mode and waits for interrupts.
*   **`ASM_funct.s`**: Contains assembly language functions.
    *   `totale_pressioni_con_filtro`: Calculates the sum of values in an array (vector of bytes), filtering them against a specified min/max range. 
        *   Inputs: `r0` (array pointer), `r1` (array length), `r2` (max filter value), `r3` (min filter value).
        *   Output: `r0` (sum of filtered values).

### Tools

*   **IDE:** Keil MDK (uVision)
*   **Compiler:** ARM Compiler
*   **Debugger:** J-Link

## File Structure

```
.
├── ARM_18052022.pdf            # Requirements
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
2.  **Build Project:** Compile and build the project from the IDE.
3.  **Hardware Setup:**
    *   Connect the LPC17xx development board to the computer.
    *   Ensure J-Link debugger is connected if using it for flashing/debugging.
4.  **Flash Firmware:** Load the compiled firmware (`.axf` file) to the microcontroller using Keil MDK's flash utility or a J-Link programmer.
5.  **Run:** Reset the microcontroller to start the application.

## Key Functionalities

Based on `sample.c`:

*   **LED Control:**
    *   Initialization of multiple LEDs.
    *   Circular shifting of the active LED using a button (KEY1).
    *   **Button Input:**
    *   Initialization of push-buttons with external interrupt capabilities.
    *   Debouncing is handled within the button driver and RIT ISR.
*   **Timer Usage:**
    *   Configuration of multiple general-purpose timers with specific intervals. (Note: `sample.c` configures them but doesn't enable them by default).
*   **Repetitive Interrupt Timer (RIT):**
    *   Initialized for a 50ms periodic interrupt, used for tasks like button debouncing and other periodic checks (in the reqs track the tenths of second).
*   **Power Management:**
    *   The system enters a low-power (power-down) mode and waits for interrupts (`wfi`) to conserve energy.
*   **Simulator Support:** Code includes conditional compilation for a simulator environment.

## Notes
* The `asciilib.crf`, `core_cm3.crf` etc. files are cross-reference files generated by the Keil IDE.
* `JLink Regs CM3.txt`, `JLinkLog.txt`, `JLinkSettings.ini` are related to the J-Link debugger.
* `ExtDll.iex` is an initialization file for an external DLL, possibly for debugging or simulation.
* `EventRecorderStub.scvd` is related to Keil's Event Recorder.

---
