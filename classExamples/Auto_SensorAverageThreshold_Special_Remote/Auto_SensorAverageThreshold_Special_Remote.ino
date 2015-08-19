
#include <microM.h>   // Use the microM library for motor control, IR command
int stepsize = 50/2;
int slowspeedL=45;
int slowspeedR=50;
int forwardSpeedL=225/2;
int forwardSpeedR=250/2;
const int yellow=40;
const int blue=37;
const int red=38;
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
int sharpl = 7;
int sharpr = 9;

int Mode;
const int AUTOMODE =0;
const int MANUALMODE =1;
const int CALMODE =2;

enum SensorPosition {
  SenseLeft=0, 
  SenseCenter=1,
  SenseRight=2,
}; 
const int numSensors=3;
//
// Sensing pins
//
const int sensePin[3]={5,3,4};  // array containing the pin numbers
// for right,center, and left

const int senseOnOffPin=1;

int sensorInput[3]={0}; // sensor inputs

int counter=0;
const int numSamples=30  ;  // 20;

// collect multiple samples for each sensor
unsigned int sensorSamples[numSensors][numSamples]={0};

const int DISTANCE_CLOSE=0;
const int DISTANCE_MEDIUM=1;
const int DISTANCE_FAR=2;

unsigned int zeroOffset[numSensors]={320, 320, 320};

void setup()
{
  pinMode(11, OUTPUT);
  microM.Setup(); // set up the library
  pinMode(senseOnOffPin,OUTPUT); // set up sensor on/off pin
  Serial.begin(9600); // set the serial port speed.
  digitalWrite(senseOnOffPin, HIGH);  // Turn on the sensor
}

/**
 * average the last "numSample" samples
 **/
void computeSampleAverage(unsigned int *averagedSamples)
{
  int sampleIndex=counter % numSamples;

  // collect raw data into sample array
  for (int i = 0; i < numSensors; i++)
  {
    sensorSamples[i][sampleIndex]=analogRead(sensePin[i]);
    ;
    averagedSamples[i]=0; // clear the computed average
    for (int j=0; j < numSamples; j++)
    {
      // compute average samples for the sensor
      averagedSamples[i] += sensorSamples[i][j];
    }
    averagedSamples[i]=averagedSamples[i]/numSamples;
  }
  counter++;
}

void applyThresholds(unsigned int *averageInput, int *normalizedValues)
{
  const int closeThreshold[numSensors]={80,80,80};
  const int mediumThreshold[numSensors]={20,20,20};
  
  for (int i = 0; i < numSensors; i++)
  {
    normalizedValues[i]=averageInput[i] - zeroOffset[i];  // - 320

  
    if (normalizedValues[i] > closeThreshold[i])
    {
      normalizedValues[i] = DISTANCE_CLOSE;   //  = 0
    } else if (normalizedValues[i] > mediumThreshold[i])
    {
      normalizedValues[i] = DISTANCE_MEDIUM;   //  = 1
    } else
    {
      normalizedValues[i] = DISTANCE_FAR;    //  = 2
    }
    
  }
}

/**
 * Determine whether or not the values changed
 **/
bool valuesChanged(int *values)
{
  bool changed=false;
  static int lastValues[numSensors]={0,0,0};
  for (int i =0; i < numSensors; i++)
  {
     if (values[i] != lastValues[i])
     {
       changed=true;
       lastValues[i]=values[i]; // set the last value
     }
  }
  return changed;
}

/**
 * Calibrate the zero level.  Point the sensors at nothing
 * then run this calibration function to set the zero level. 
 **/
void calibrateZeroLevel()
{
  Serial.print("Calibrating zero level with ");
  Serial.print(numSamples);
  Serial.print(" samples");
  //
  // collect numSamples first, then compute the average
  //
  for (int n=0; n < numSamples; n++)
  {
    for (int i = 0; i < numSensors; i++)
    {
      sensorSamples[i][n]=analogRead(sensePin[i]);
    }
    delay(20); // wait a bit between reads

    Serial.print(".");
  }
  Serial.println();
  // fill in the zero offset with the sample average.
  computeSampleAverage(zeroOffset);
  Serial.print("Calibrated zero offsets:  ");
  for (int i = 0; i < numSensors; i++)
  {
    Serial.print(zeroOffset[i]);
    Serial.print(" ");
  }  
  Serial.println();
}

void loop()
{
  switch (microM.ircommand)
  {
    case yellow :      
        Mode = AUTOMODE;
        break;
      
  
    case blue :
        
      Mode = MANUALMODE;
      break;
     
    case red :
      
      Mode = CALMODE;
      break;
   
  }
  
  switch(Mode) 
  {
      case AUTOMODE :
        digitalWrite(11, LOW);
        automode();
        break;

      case MANUALMODE :
        digitalWrite(11, HIGH);
         manualmode();
        break;
      
      case CALMODE : 
        longBlinks();  
        break;

  }
  } 
void manualmode() 
{
  if (microM.ircommand == 117 ) {
      microM.Motors(forwardSpeedL,forwardSpeedR,0,0);
  }

   if (microM.ircommand == Left ) {
      microM.Motors(0,255,0,0);
  }
   if (microM.ircommand == Right ) {
      microM.Motors(205,0,0,0);
  }
   if (microM.ircommand == Down ) {
      microM.Motors(-205,-205,0,0);
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
   if (microM.ircommand == sharpl ) {
      microM.Motors(-slowspeedL,slowspeedR,0,0);
  }
   if (microM.ircommand == sharpr ) {
      microM.Motors(slowspeedL,-slowspeedR,0,0);
  }
}
void automode()
 {
  unsigned int averageInput[numSensors]={0};
  int normalizedValues[numSensors]={0};
  const int menuButton=97;
  //
  // ircommand sometimes produces spurrious values, so
  // we add a allowCalibrate variable to guard against this.
  // press topMenu to allow calibration, then press menu to 
  // calibrate
  //
  const int displayButton=59; // Lock calibrate
  const int topMenuButton=89; // Allow calibrate
  static bool allowCalibrate=false;

  {
    computeSampleAverage(averageInput);
        // Serial.print("averageInput[2] = ");
       // Serial.println(averageInput[2]);        
  
      applyThresholds(averageInput, normalizedValues);
        
      if (normalizedValues[0] == 2 && normalizedValues[1] == 2 && normalizedValues[2] == 2) { //Far Sense
        forwardSpeedR=250/2;   
        forwardSpeedL=225/2;      
      } 

      if (normalizedValues[0] == 0 || normalizedValues[1] == 0 || normalizedValues[2] == 0) { //Close Sense 
          if (normalizedValues[1] == 0) { //Close Sense
              Slow_Left_Wheel(stepsize*2);
              Slow_Right_Wheel(stepsize*2);
          }
          if (normalizedValues[2] == 0) { // if Right is Close Sense
            Fast_Right_Wheel(stepsize*2);
            Slow_Left_Wheel(stepsize*2);
          }       
          else if (normalizedValues[0] == 0) { // if Left is Close Sense
            Fast_Left_Wheel(stepsize*2);
            Slow_Right_Wheel(stepsize*2);
          }                     
            
      } else 
      
      {
          
            if (normalizedValues[1] == 1) { //if center is Medium Sense
              Slow_Left_Wheel(stepsize);
              Slow_Right_Wheel(stepsize);
            } 
            if (normalizedValues[2] == 1) { //if Right is Medium Sense
              Slow_Left_Wheel(stepsize);
              Fast_Right_Wheel(stepsize);
            } 
            else if (normalizedValues[0] == 1) { // if Left is Medium Sense
              Fast_Left_Wheel(stepsize);
              Slow_Right_Wheel(stepsize);
            }       
           
        }
        microM.Motors(forwardSpeedL,forwardSpeedR,0,0); 
        Serial.println();
        allowCalibrate=false;
      }

 }
void Slow_Left_Wheel(int step)
{
  
    forwardSpeedL=forwardSpeedL-step;
    if (forwardSpeedL < -205/2 )
    {
      forwardSpeedL= -205/2;
    }
}

void Fast_Left_Wheel(int step)
{
    forwardSpeedL=forwardSpeedL+step;
    if (forwardSpeedL > 225/2 )
    {
      forwardSpeedL= 225/2;
    }
}

void Slow_Right_Wheel(int step)
{
  forwardSpeedR=forwardSpeedR-step; 
  if (forwardSpeedR < -205/2 ) 
  {
    forwardSpeedR=-205/2;
  }
}

void Fast_Right_Wheel(int step)
{

  forwardSpeedR=forwardSpeedR+step;
  if (forwardSpeedR > 250/2 ) 
  {
    forwardSpeedR=250/2;
  }
}
void longBlinks()
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
    digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
  }

}  // long blink

