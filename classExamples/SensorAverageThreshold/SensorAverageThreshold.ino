
#include <microM.h>   // Use the microM library for motor control, IR command

enum SensorPosition {
  SenseLeft=0, 
  SenseCenter=1,
  SenseRight=2,
}; 
const int numSensors=3;
//
// Sensing pins
//
const int sensePin[3]={5,4,3};  // array containing the pin numbers
// for right,center, and left

const int senseOnOffPin=1;

int sensorInput[3]={0}; // sensor inputs

int counter=0;
const int numSamples=30;

// collect multiple samples for each sensor
unsigned int sensorSamples[numSensors][numSamples]={0};

const int DISTANCE_CLOSE=0;
const int DISTANCE_MEDIUM=1;
const int DISTANCE_FAR=2;

unsigned int zeroOffset[numSensors]={320, 320, 320};

void setup()
{
  microM.Setup(); // set up the library
  pinMode(senseOnOffPin,OUTPUT); // set up sensor on/off pin
  Serial.begin(19200); // set the serial port speed.
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
  const int closeThreshold[numSensors]={400,400,400};
  const int farThreshold[numSensors]={100, 99, 70};
  const int mediumThreshold[numSensors]={200,200,200};
  
  for (int i = 0; i < numSensors; i++)
  {
    normalizedValues[i]=averageInput[i] - zeroOffset[i];
    if (normalizedValues[i] > closeThreshold[i])
    {
      normalizedValues[i] = DISTANCE_CLOSE;
    } else if (normalizedValues[i] > mediumThreshold[i])
    {
      normalizedValues[i] = DISTANCE_MEDIUM;
    } else
    {
      normalizedValues[i] = DISTANCE_FAR;
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
  unsigned int averageInput[numSensors]={0};
  int normalizedValues[numSensors]={0};
  const int menuButton=28;
  //
  // ircommand sometimes produces spurrious values, so
  // we add a allowCalibrate variable to guard against this.
  // press topMenu to allow calibration, then press menu to 
  // calibrate
  //
  const int displayButton=85; // Lock calibrate
  const int topMenuButton=27; // Allow calibrate
  static bool allowCalibrate=false;
  
  if (microM.ircommand > 0)
  {
    switch (microM.ircommand)
    {
      case topMenuButton:
        allowCalibrate = true;
        Serial.println("Allow Calibrate");
        break;
      case displayButton:
        allowCalibrate=false;
        Serial.println("Calibration Locked");
        break;
      case menuButton:
        if (allowCalibrate)
        {
          calibrateZeroLevel();
          microM.ircommand = 0;
          allowCalibrate=false;
        }
        break;
    }
    microM.ircommand=0;
  } else
  {
    computeSampleAverage(averageInput);

    applyThresholds(averageInput, normalizedValues);

    if (valuesChanged(normalizedValues))
    {
      for (int i = 0; i < numSensors; i++)
      {
        Serial.print(normalizedValues[i]);
        Serial.print(" ");
      }
      Serial.println();
      allowCalibrate=false;
    }
  } 
}


