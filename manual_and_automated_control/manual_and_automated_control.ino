#include <microM.h>
int forwardSpeedL=205;
int forwardSpeedR=-250;
int slowspeedL=45;
int slowspeedR=-110;

const int leftPin=5;
const int centerPin=3;
const int rightPin=4;
const int senseOnOffPin=1;

int Left =  53;
int Right = 52;
int Up = 117;
int Down = 118;
int Enter = 102;
int Play = 27;
int Fastfwd = 29;
int Reverse = 28;
int Pause = 26;
int Stop = 25;
int slowforward =  2;
int slowleft = 4;
int slowright = 6;

void setup()
{
  microM.Setup();
  pinMode(senseOnOffPin,OUTPUT); // set up sensor on/off pin

  Serial.begin(9600); // set the serial port speed.
  digitalWrite(senseOnOffPin, HIGH); // Turn on the sensor    
}

void loop()
{  
  

  if(microM.ircommand == Play) 
  {         
    Serial.println ("Auto Mode Now  ...");
    Auto_Mode();
    microM.Motors(forwardSpeedL,forwardSpeedR,0,0); 
  }
  else 
  {
    Serial.println ("Manual Mode Now  ...");
    Serial.println (microM.ircommand);
    Manual_Mode();

  }
  
  
  
}

void Manual_Mode()
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


void Auto_Mode()
{
  int step_size=50;

     if(Detect_Center() < 300 && Detect_Left() < 300 && Detect_Right() < 300 )
     {
        forwardSpeedL=205;
        forwardSpeedR=-250;
     }
     else {
       if(Detect_Center() > 500)
       {
          Serial.println("Detected Center!!!");
          Slow_Right_Wheel(step_size*2);
          Slow_Left_Wheel(step_size/2);
       }
  
       if(Detect_Left() > 700)
       {
          Serial.println("Detected Left!!!");
          Slow_Right_Wheel(step_size);
          Fast_Left_Wheel(step_size);
       }
  
       if(Detect_Right() > 500)
       {
          Serial.println("Detected Right!!!");
          Slow_Left_Wheel(step_size);
          Fast_Right_Wheel(step_size);
       }
     }
     
}

int Detect_Left()
{
  int sensorInput=0;
  
  sensorInput=analogRead(leftPin); //left
  return  sensorInput;
}

int Detect_Right()
{
  int sensorInput=0;
  
  sensorInput=analogRead(rightPin); //left
  return  sensorInput;
}

int Detect_Center()
{
  int sensorInput=0;
  
  sensorInput=analogRead(centerPin); //left
  return  sensorInput;
}


void Slow_Left_Wheel(int step)
{
  
    forwardSpeedL=forwardSpeedL-step;
    if (forwardSpeedL < -200 )
    {
      forwardSpeedL= -200;
    }
}

void Fast_Left_Wheel(int step)
{
    forwardSpeedL=forwardSpeedL+step;
    if (forwardSpeedL > 205 )
    {
      forwardSpeedL=205;
    }
}

void Slow_Right_Wheel(int step)
{
  forwardSpeedR=forwardSpeedR+step; 
  if (forwardSpeedR > 370 ) 
  {
    forwardSpeedR=370;
  }
}

void Fast_Right_Wheel(int step)
{

  forwardSpeedR=forwardSpeedR-step;
  if (forwardSpeedR < -250 ) 
  {
    forwardSpeedR=-250;
  }
}


