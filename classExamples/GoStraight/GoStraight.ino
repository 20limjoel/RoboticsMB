/*
This program demonstrates the use of the Micro Magician dual H bridge.
The DC motor control function can independantly control 2 small DC motors.

This program will gradually increase the speed of the 2 motors and then
engage the brake when they are at full speed.
The motors will then reverse direction and repeate the process.

Written by Russell Cameron
*/
#include <microM.h>


void setup()
{ 
  microM.Setup();
  Serial.begin(19200); // set the serial port speed.
  microM.ircommand=10; // stopped.
}

void goStraight(int speed)
{
  if (speed == 0)
  {
     microM.Motors(0,0,1,1); // put on the brakes!
  } else
  {
     microM.Motors(speed,speed,0,0);    // update motor controller
  }
}

void loop()
{
  static int speed=0;
  const int minSpeed=25;
  const int ZERO_KEY_CODE=10;
  const int STOP_KEY_CODE=57;
  
  Serial.println(speed);
  if (microM.ircommand == ZERO_KEY_CODE || microM.ircommand == STOP_KEY_CODE) // key 0 is to stop
  {
    speed=0;
  } else if (microM.ircommand > 0 && microM.ircommand < 10)
  {
    // compute the speed based on the ircommand value.
    speed=microM.ircommand*50 - 255;
    // set the minimum speed
    if (abs(speed) < minSpeed)
    {
      if (speed > 0)
      {
        speed = minSpeed;
      } else
      {
        speed = -1*minSpeed;
      }
    }
  }
  goStraight(speed);
}
