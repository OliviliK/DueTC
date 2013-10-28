#include <DueTC.h>

/*
  DueTC_Demo.ino - Testing and demonstration of DUE Timer Control implemented in DueTC.cpp
 
 Created by Olavi Kamppari, October 2013.
 
 Test the following routines
 - setupTC_Pin_Timing      with commandline       Sp period clock
 - changeTC_Pin_Period     with commandline       Cp period
 - setupTC_Interrupt       with commandline       In period clock
 - changeTC2_Period        with commandline       Pn period
 - NVIC_SetPendingIRQ      with commandline       Fn                (this is an inline macro and not specified in DueTC.cpp)
 
 where
      Pin number p is 2,3,5, or 11
      Period is 1-65535
      Clock is 0-4
      Interrupt number n is 2,3,4,5

 In the test program the user specified interrupt handlers are toggling outputs in pins 6, 7, 8, and 9
 
 In the setup routine the Pin5 setting and the TC2-TC5 settings the times are set equal using different master clock dividers.
 With a scope connected to pins 5-9, it can be observed that pins 5-8 are are providing the same timing and are staying in synch.
 Pin 9 has the "same" timing, but the SLCK based timing is not staying in synch with MCK.

 Released into the public domain.
*/



void togglePin4(){
  static byte flg;
  digitalWrite(4,flg=!flg);
}

void togglePin6(){
  static byte flg;
  digitalWrite(6,flg=!flg);
}

void togglePin7(){
  static byte flg;
  digitalWrite(7,flg=!flg);
}

void togglePin8(){
  static byte flg;
  digitalWrite(8,flg=!flg);
}

void togglePin9(){
  static byte flg;
  digitalWrite(9,flg=!flg);
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(100);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);

                                           // Target toggling frequency: 84,000 kHz/3,072 = 27.3 kHz
  setupTC_Pin5_Timing(1536,0);             // 84,000,000/(2*1536) = 27,343.75 Hz (36.57 us pulse)
  setupTC2_Interrupt(384,1,togglePin6);    // 84,000,000/(8*384)  = 27,343.75 Hz (36.57 us pulse)
  setupTC3_Interrupt(96,2,togglePin7);     // 84,000,000/(32*96)  = 27,343.75 Hz (36.57 us pulse)
  setupTC4_Interrupt(24,3,togglePin8);     // 84,000,000/(128*24) = 27,343.75 Hz (36.57 us pulse)
  setupTC5_Interrupt(1,4,togglePin9);      // 84,000,000/(32*96)  = 27,343.75 Hz (36.57 us pulse)
}

void loop() {
  char buffer[255];
  byte bLen;
  byte delimPos[10];
  int arg[10];
  byte argCount;
  byte i;
  char command;
  char delim;
  int pinNr;
  int timerNr;
  unsigned int period;
  byte tcClock;

  bLen=Serial.readBytesUntil('\n', buffer, sizeof(buffer));
  if (bLen>0) {
    buffer[bLen]=0;                     // set end of string marker after last character
    command=buffer[0];
    delimPos[0]=0;                      // mark first argument to start after command
    argCount=1;
    for (i=1;i<bLen;i++) {              // replace all delimitesr with zeroes
      delim=buffer[i];
      if (delim<'0' || delim>'9') { 
        buffer[i]=0;
        delimPos[argCount++]=i;
        if (argCount>=sizeof(delimPos)) break;
      }
    }
                                        // get argument values 
    for (i=0;i<argCount;i++) arg[i]=atoi(&buffer[delimPos[i]+1]);
    Serial.print(command);
    for (i=0;i<argCount;i++) {          // echo the command line back
      Serial.print(arg[i]);
      Serial.print(" ");
    }
    Serial.println();
    switch (command) {
    case 's':
    case 'S':
      pinNr=arg[0];
      period=(argCount>1)?arg[1]:0;
      tcClock=(argCount>2)?arg[2]:0;
      switch (pinNr) {
      case 2:  
        setupTC_Pin2_Timing(period,tcClock); 
        break;
      case 5:  
        setupTC_Pin5_Timing(period,tcClock); 
        break;
      case 3:  
        setupTC_Pin3_Timing(period,tcClock); 
        break;
      case 11: 
        setupTC_Pin11_Timing(period,tcClock); 
        break;
      default:
        Serial.print("Not supported pin ");
        Serial.println(pinNr);
        Serial.println("Available pins are: 2, 3, 5, 11");
      }
      break;
    case 'c':
    case 'C':
      pinNr=arg[0];
      period=(argCount>1)?arg[1]:0;
      switch (pinNr) {
      case 2:  
        changeTC_Pin2_Period(period); 
        break;
      case 5:  
        changeTC_Pin5_Period(period); 
        break;
      case 3:  
        changeTC_Pin3_Period(period); 
        break;
      case 11: 
        changeTC_Pin11_Period(period); 
        break;
      default:
        Serial.print("Not supported pin ");
        Serial.println(pinNr);
        Serial.println("Available pins are: 2, 3, 5, 11");
      }
      break;
    case 'i':
    case 'I':
      timerNr=arg[0];
      period=(argCount>1)?arg[1]:0;
      tcClock=(argCount>2)?arg[2]:0;
      switch (timerNr) {
      case 2:  
        setupTC2_Interrupt(period,tcClock,togglePin6); 
        break;
      case 3:  
        setupTC3_Interrupt(period,tcClock,togglePin7); 
        break;
      case 4:  
        setupTC4_Interrupt(period,tcClock,togglePin8); 
        break;
      case 5:  
        setupTC5_Interrupt(period,tcClock,togglePin9); 
        break;
      default:
        Serial.print("Not supported timer ");
        Serial.println(timerNr);
        Serial.println("Available interrupts are: 2, 3, 4, 5");
      }      
      break;
    case 'p':
    case 'P':
      timerNr=arg[0];
      period=(argCount>1)?arg[1]:0;
      switch (timerNr) {
      case 2:  
        changeTC2_Period(period); 
        break;
      case 3:  
        changeTC3_Period(period); 
        break;
      case 4:  
        changeTC4_Period(period); 
        break;
      case 5:  
        changeTC5_Period(period); 
        break;
      default:
        Serial.print("Not supported timer ");
        Serial.println(timerNr);
        Serial.println("Available interrupts are: 2, 3, 4, 5");
      }      
      break;
    case 'f':
    case 'F':
      timerNr=arg[0];
      switch (timerNr) {
      case 2:  
        NVIC_SetPendingIRQ(TC2_IRQn); 
        break;
      case 3:  
        NVIC_SetPendingIRQ(TC3_IRQn); 
        break;
      case 4:  
        NVIC_SetPendingIRQ(TC4_IRQn); 
        break;
      case 5:  
        NVIC_SetPendingIRQ(TC5_IRQn); 
        break;
      default:
        Serial.print("Not supported timer ");
        Serial.println(timerNr);
        Serial.println("Available interrupts are: 2, 3, 4, 5");
      }
      break;
    default:
      Serial.print("Unknown command");
      Serial.println(command);
      Serial.println("Available commands are:");
      Serial.println("   Sp period clock      to setup pin p (2,3,5,11) for period (1-65535) with clock (0-4)");
      Serial.println("   Cp period            to change pin p (2,3,5,11) period (1-65535)");
      Serial.println("   In period clock      to init interrupt n (2,3,4,5) for period (1-65535) with clock (0-4)");
      Serial.println("   Pn period            to change interrupt n (2,3,4,5) period (1-65535)");
      Serial.println("   Fn                   to force an interrupt n (2,3,4,5)");
    }
  }
}

