DueTC
=====

Arduino DUE Timer Control for pins and interrupts

Copy the DueTC.h and DueTC.cpp into your Arduino library.  Open the DueTC_Demo.ino in Arduino IDE to play with DueTC.

The DueTC_Demo assumes the usage of an oscilloscope to observe the high speed responses.  Modify the user defined interrupt handlers
for different visualization.  For example change, copy togglePin9 function, change its name to togglePin13, change the pin number to 13, and in setupTC5_interrupt, change the togglePin9 to togglePin9.  Obviously you can just change the pin number, but then the naming will be misleading.

The demo is interactive and is controlled by commands given in Serial Monitor.  The commands are
 - setupTC_Pin_Timing      with commandline       Sp period clock
 - changeTC_Pin_Period     with commandline       Cp period
 - setupTC_Interrupt       with commandline       In period clock
 - changeTC2_Period        with commandline       Pn period
 - NVIC_SetPendingIRQ      with commandline       Fn                (this is an inline macro and not specified in DueTC.cpp)
 
 where
  - Pin number p is 2,3,5, or 11
  - Period is 1-65535
  - Clock is 0-4
  - Interrupt number n is 2,3,4,5

  
Pin Control
-----------

The time controlled pin functions are
 - void setupTC_Pin2_Timing(unsigned int period, byte tcClock);
 - void changeTC_Pin2_Period(unsigned int period);

 - void setupTC_Pin5_Timing(unsigned int period, byte tcClock);
 - void setupTC_Pin3_Timing(unsigned int period, byte tcClock);
 - void setupTC_Pin11_Timing(unsigned int period, byte tcClock);


 - void changeTC_Pin5_Period(unsigned int period);
 - void changeTC_Pin3_Period(unsigned int period);
 - void changeTC_Pin11_Period(unsigned int period);

Interrupt Control
-----------------

 - void setupTC2_Interrupt(unsigned int period, byte tcClock, void (*isr)());
 - void setupTC3_Interrupt(unsigned int period, byte tcClock, void (*isr)());
 - void setupTC4_Interrupt(unsigned int period, byte tcClock, void (*isr)());
 - void setupTC5_Interrupt(unsigned int period, byte tcClock, void (*isr)());

 - void changeTC2_Period(unsigned int period);
 - void changeTC3_Period(unsigned int period);
 - void changeTC4_Period(unsigned int period);
 - void changeTC5_Period(unsigned int period);
 
Restrictions
------------

Only a subset of pins are supported due to internal wiring in CPU.  There are 3 timer blocks in CPU and each has 3 channels.  The 9 timers are allocated in following way:

- 0, for square wave generation in pin 2
- 1, is used for system services (such as milli, micros, and delay)
- 2-5, are used for time based interrupts
- 6-8, for square wave generation in pins 5, 3, and 11

Additional restrictions will be caused by support of two wire phase encoders and similar timer/counter based applications.

