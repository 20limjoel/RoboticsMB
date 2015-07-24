/*
This program demonstrates the use of the Micro Magician dual H bridge.
The DC motor control function can independantly control 2 small DC motors.

This program will gradually increase the speed of the 2 motors and then
engage the brake when they are at full speed.
The motors will then reverse direction and repeate the process.

Written by Russell Cameron
*/
#include <microM.h>
int forwardSpeedL=205;
int forwardSpeedR=-255;
int slowspeedL=45;
int slowspeedR=-110;
int Left =  53;
int Right = 52;
int Up = 117;
int Down = 118;
int Enter = 102;
int Play = 27;
int Pause = 26;
int Stop = 25;
int slowforward =  2;
int slowleft = 4;
int slowright = 6;
void setup()
{ 
  microM.Setup();
  Serial.begin(19200); // set the serial port speed.
  microM.ircommand=10; // stopped.
   Serial.begin(9600);                            // initialize serial monitor

}

void goStraight(int speed)
{
  if (speed == 0)
  {
     microM.Motors(0,0,0,0); // put on the brakes!
  } else
  {
     microM.Motors(0,0,0,0);    // update motor controller
  }
}

void loop()
{
  Serial.println(microM.ircommand,DEC);
  {
      if (microM.ircommand == 117 ) {
          microM.Motors(forwardSpeedL,forwardSpeedR,0,0);
      }

       if (microM.ircommand == Left ) {
          microM.Motors(0,-255,0,0);
      }
       if (microM.ircommand == Right ) {
          microM.Motors(205,0,0,0);
      }
       if (microM.ircommand == Down ) {
          microM.Motors(-200,370,0,0);
      }
      if (microM.ircommand == Enter ) {
          microM.Motors(0,0,0,0);
      }
       if (microM.ircommand == slowforward ) {
          microM.Motors(slowspeedL,slowspeedR,0,0);
      }
          if (microM.ircommand == slowleft ) {
          microM.Motors(0,slowspeedR,0,0);
      }
       if (microM.ircommand == slowright ) {
          microM.Motors(slowspeedL,0,0,0);
      }
  }

}
