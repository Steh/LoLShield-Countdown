/*
 Basic LoL Shield Test
 
 Writen for the LoL Shield, designed by Jimmie Rodgers:
 http://jimmieprodgers.com/kits/lolshield/
 
 This needs the Charliplexing library, which you can get at the
 LoL Shield project page: http://code.google.com/p/lolshield/
 
 Created by Jimmie Rodgers on 12/30/2009.
 Adapted from: http://www.arduino.cc/playground/Code/BitMath
 
 History:
  	December 30, 2009 - V1.0 first version written at 26C3/Berlin

  This is free software; you can redistribute it and/or
  modify it under the terms of the GNU Version 3 General Public
  License as published by the Free Software Foundation; 
  or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <avr/pgmspace.h>  //AVR library for writing to ROM
#include <Charliplexing.h> //Imports the library, which needs to be
                           //Initialized in setup.

//Sets the time each frame is shown (milliseconds)
const unsigned int blinkdelay = 2000;

PROGMEM const uint16_t BitMapCountdown[][9] = {
//Diaganal swipe across the screen
{0, 3980, 8142, 6342, 6342, 6342, 8190, 4092, 0}, // 9
{0, 3900, 6630, 4290, 4290, 4290, 6630, 3900, 0}, // 8
{0, 6144, 6150, 6174, 6264, 6624, 8064, 7680, 0}, // 7
{0, 508, 2046, 3782, 7366, 6342, 254, 124, 0},    // 6
{0, 8076, 8078, 6534, 6534, 6598, 6398, 6268, 0}, // 5
{0, 224, 992, 1888, 3680, 8190, 8190, 96, 0},     // 4
{0, 3084, 7182, 6342, 6342, 6342, 4092, 1848, 0}, // 3
{0, 3078, 7198, 6206, 6390, 6630, 8070, 3846, 0}, // 2
{0, 3072, 7168, 7168, 7168, 8190, 8190, 4094, 0}, // 1
{0, 4092, 8190, 6150, 6150, 6150, 8190, 4092, 0}, // 0	
{18000} // Killswitch
};

void setup() {
  LedSign::Init(DOUBLE_BUFFER | GRAYSCALE);  //Initializes the screen
}
void loop() {
      DisplayBitMap(1);  //Displays the bitmap
}

void DisplayBitMap(uint8_t grayscale)
{
  boolean run=true;    //While this is true, the screen updates
  byte frame = 0;      //Frame counter
  byte line = 0;       //Row counter
  unsigned long data;  //Temporary storage of the row data
  unsigned long start = 0;

  while(run == true) {

    for(line = 0; line < 9; line++) {

      //Here we fetch data from program memory with a pointer.
      data = pgm_read_word_near (&BitMapCountdown[frame][line]);
      
      //Kills the loop if the kill number is found
      if (data==18000){
        run=false;
      }
      
      //This is where the bit-shifting happens to pull out
      //each LED from a row. If the bit is 1, then the LED
      //is turned on, otherwise it is turned off.
      else for (byte led=0; led<14; ++led) {
        if (data & (1<<led)) {
          LedSign::Set(led, line, grayscale);
        }
        else {
          LedSign::Set(led, line, 0);
        }
      }
    }

    LedSign::Flip(true);   
    
    //unsigned long end = millis();
    //if ( start && (diff < blinkdelay) )
    delay( blinkdelay );
    //start = end;

    frame++;  
  }
}
