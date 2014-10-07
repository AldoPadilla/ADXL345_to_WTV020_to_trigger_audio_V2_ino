/*
ADXL345 to trigger WTV020-SD-16P module to play tracks all connected to an Arduino board.
 Mashed up by Aldo Padilla, September 23rd, 2014.
 */
 
/*****************************************************************************/

//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/

#include <Wire.h>
#include <ADXL345.h>
#include <Wtv020sd16p.h>
int resetPin = 2;  // The pin number of the reset pin.
int clockPin = 3;  // The pin number of the clock pin.
int dataPin = 6;  // The pin number of the data pin.
int busyPin = 7;  // The pin number of the busy pin.
/*
Create an instance of the Wtv020sd16p class.
 1st parameter: Reset pin number.
 2nd parameter: Clock pin number.
 3rd parameter: Data pin number.
 4th parameter: Busy pin number.
 */
Wtv020sd16p wtv020sd16p(resetPin,clockPin,dataPin,busyPin);

long randNumber;


ADXL345 adxl; //variable adxl is an instance of the ADXL345 library

void setup(){
  
      //Initializes the module.
  wtv020sd16p.reset();
  
  Serial.begin(9600);
  adxl.powerOn();

  //set activity/ inactivity thresholds (0-255)
  adxl.setActivityThreshold(75); //62.5mg per increment
  adxl.setInactivityThreshold(75); //62.5mg per increment
  adxl.setTimeInactivity(25); // how many seconds of no activity is inactive?
 
  //look of activity movement on this axes - 1 == on; 0 == off 
  adxl.setActivityX(1);
  adxl.setActivityY(1);
  adxl.setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
  adxl.setInactivityX(1);
  adxl.setInactivityY(1);
  adxl.setInactivityZ(1);
 
  //look of tap movement on this axes - 1 == on; 0 == off
  adxl.setTapDetectionOnX(0);
  adxl.setTapDetectionOnY(0);
  adxl.setTapDetectionOnZ(1);
 
  //set values for what is a tap, and what is a double tap (0-255)
  adxl.setTapThreshold(50); //62.5mg per increment
  adxl.setTapDuration(15); //625us per increment
  adxl.setDoubleTapLatency(80); //1.25ms per increment
  adxl.setDoubleTapWindow(200); //1.25ms per increment
 
  //set values for what is considered freefall (0-255)
  adxl.setFreeFallThreshold(5); //(5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(20); //(20 - 70) recommended - 5ms per increment
 
  //setting all interrupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );
 
  //register interrupt actions - 1 == on; 0 == off  
  adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  adxl.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  adxl.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);
}

void loop(){
  
	//Boring accelerometer stuff   
	int x,y,z;  
	adxl.readXYZ(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z
	// Output x,y,z values 
	Serial.print("values of X , Y , Z: ");
	Serial.print(x);
	Serial.print(" , ");
	Serial.print(y);
	Serial.print(" , ");
	Serial.println(z);
	
	double xyz[3];
	double ax,ay,az;
	adxl.getAcceleration(xyz);
	ax = xyz[0];
	ay = xyz[1];
	az = xyz[2];
	Serial.print("X=");
	Serial.print(ax);
    Serial.println(" g");
	Serial.print("Y=");
	Serial.print(ay);
    Serial.println(" g");
	Serial.print("Z=");
	Serial.println(az);
    Serial.println(" g");
	Serial.println("**********************");
	delay(500);




  //Fun Stuff!    
  //read interrupts source and look for triggerd actions
  
  //getInterruptSource clears all triggered actions after returning value
  //so do not call again until you need to recheck for triggered actions
   byte interrupts = adxl.getInterruptSource();
  
  // freefall
  if(adxl.triggered(interrupts, ADXL345_FREE_FALL)){
    Serial.println("freefall");
    //add code here to do when freefall is sensed
  } 
  
  //inactivity
  if(adxl.triggered(interrupts, ADXL345_INACTIVITY)){
    Serial.println("inactivity");
          randNumber = random(0, 4);
     //add code here to do when inactivity is sensed
      wtv020sd16p.asyncPlayVoice(randNumber);
      delay (1000);
  }
  
  //activity
  if(adxl.triggered(interrupts, ADXL345_ACTIVITY)){
    Serial.println("activity"); 
     //add code here to do when activity is sensed
  }
  
  //double tap
  if(adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)){
    Serial.println("double tap");
      randNumber = random(5, 8);
     //add code here to do when a 2X tap is sensed
      wtv020sd16p.asyncPlayVoice(randNumber);
      delay (1000);
  }
  
  //tap
  if(adxl.triggered(interrupts, ADXL345_SINGLE_TAP)){
    Serial.println("tap");
     //add code here to do when a tap is sensed
  }
  
  //Run Motion
  if(x > 90 && y >= 10 && y <= 100 && ax >= 0.65 && az >= 0.70){
    Serial.println("RUN");
          randNumber = random(9, 14);
     //add code here to do when inactivity is sensed
      wtv020sd16p.asyncPlayVoice(randNumber);
      delay (1000);
  }  
  
    //Quick Turn
  if((y >= 100 || y <= -100) && ay > 0.85){
    Serial.println("QUICK TURN");
          randNumber = random(15, 22);
     //add code here to do when inactivity is sensed
      wtv020sd16p.asyncPlayVoice(randNumber);
      delay (1000);
  }  
  
  
    //Jump
  if(x >= 100 && ax > 1.20){
    Serial.println("JUMP");
          randNumber = random(23, 31);
     //add code here to do when inactivity is sensed
      wtv020sd16p.asyncPlayVoice(randNumber);
      delay (1000);
  }  
  
   /*
   More WTV020 commands
   
    //Plays synchronously an audio file. Busy pin is used for this method.
  wtv020sd16p.playVoice(0);
  //Plays asynchronously an audio file.
  wtv020sd16p.asyncPlayVoice(1);
  //Plays audio file number 1 during 2 seconds.
  delay(5000);
  //Pauses audio file number 1 during 2 seconds.  
  wtv020sd16p.pauseVoice();
  delay(5000);
  //Resumes audio file number 1 during 2 seconds.
  wtv020sd16p.pauseVoice();
  delay(5000);  
  //Stops current audio file playing.
  wtv020sd16p.stopVoice();
  //Plays synchronously an audio file. Busy pin is used for this method.  
  wtv020sd16p.asyncPlayVoice(2);
  delay(2000);   
  //Mutes audio file number 2 during 2 seconds.
  wtv020sd16p.mute();
  delay(2000);
  //Unmutes audio file number 2 during 2 seconds.
  wtv020sd16p.unmute();
  delay(2000);    
  //Stops current audio file playing.
  wtv020sd16p.stopVoice(); */
   
 
}
