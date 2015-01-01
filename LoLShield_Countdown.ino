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

/*
The BitMap array is what contains the frame data. Each line is one full frame.
Since each number is 16 bits, we can easily fit all 14 LEDs per row into it.
The number is calculated by adding up all the bits, starting with lowest on
the left of each row. 18000 was chosen as the kill number, so make sure that
is at the end of the matrix, or the program will continue to read into memory.

Here PROGMEM is called, which stores the array into ROM, which leaves us
with our RAM. You cannot change the array during run-time, only when you
upload to the Arduino. You will need to pull it out of ROM, which is covered
below. If you want it to stay in RAM, just delete PROGMEM
*/
PROGMEM const uint16_t BitMap[][9] = {
//Diaganal swipe across the screen

////Null
//{0, 0, 0, 0, 0, 0, 0, 0, 0}, 
//{0, 4094, 4094, 3078, 3078, 3078, 4094, 4094, 0},
//{1, 1, 1, 1, 1, 1, 1, 1, 1}, 
{0, 4092, 8190, 3078, 3078, 3078, 8190, 4092, 0}, // NULL// N
{0, 16381, 0, 0, 0, 0, 0, 16383, 0},
//{0, 16383, 7, 0, 0, 0, 0, 16383, 0},
//{1, 0, 0, 0, 0, 0, 0, 0, 0},
//{1, 0, 0, 0, 0, 0, 0, 0, 0},
//{3, 1, 0, 0, 0, 0, 0, 0, 0},
{18000}
};

void setup() {
  LedSign::Init(DOUBLE_BUFFER | GRAYSCALE);  //Initializes the screen
}
void loop() {
  //for (uint8_t gray = 1; gray < SHADES; gray++)
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
      data = pgm_read_word_near (&BitMap[frame][line]);
      
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
