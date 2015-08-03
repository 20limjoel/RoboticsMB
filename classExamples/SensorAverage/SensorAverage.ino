
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
const int numSamples=20;

// collect multiple samples for each sensor
unsigned int sensorSamples[numSensors][numSamples]={0};

const int DISTANCE_CLOSE=0;
const int DISTANCE_MEDIUM=1;
const int DISTANCE_FAR=2;

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

void loop()
{
  unsigned int averageInput[numSensors]={0};
  
  computeSampleAverage(averageInput);

  for (int i = 0; i < numSensors; i++)
  {
    Serial.print(averageInput[i]);
    Serial.print(" ");
  }
  Serial.println();
}


