/*
 This program demonstrates the use of the Micro Magician control board IR decoding function.
 This function decodes Sony IR Codes (SIRC) and returns the 7bit command as a byte. 
 
 Included in the library are instructions for configuring the universal remote sold by DAGU.
 
 Written by Russell Cameron
 */


#include <microM.h>

unsigned long lastCmdTime=0;

void setup()
{ 
  microM.Setup();                                // this must be called to initialize timer 2
  //microM.irpin=4;                              // only required for external IR receivers. Defaults to 4 for onboard IR receiver

  Serial.begin(19200);                            // initialize serial monitor
}

enum keyCodes
{
  LEFT_KEY=124,
  RIGHT_KEY=125,
  UP_KEY=122,
  DOWN_KEY=123
};

/**
 * Convert an IR code into a computer command
 **/
const char *codeToCmd(int irCode)
{
  switch (irCode)
  {
  case LEFT_KEY:
    return "Left";
    break;
  case RIGHT_KEY:
    return "Right";
    break;
  case UP_KEY: 
    return "Up";
    break;
  case DOWN_KEY:
    return "Down";
    break;

  }  
  return NULL; // no mapping defined
}

void loop()
{
  // limit the number of key presses processed to filter out repeated commands
  const int keyPressRate=250; // keypress rate in ms
  unsigned long currentTime=millis();
  if(microM.ircommand>0 && currentTime-lastCmdTime > keyPressRate)  {
    lastCmdTime=currentTime;

    const char * cmd=codeToCmd(microM.ircommand); 
    if (cmd != NULL)
    {
      Serial.print("Cmd ");
      Serial.println(cmd);
    } 
    else
    {
      Serial.print("IRCode ");
      Serial.println(microM.ircommand,DEC);        // use the DEC option to convert a byte to a decimal number
    }
  } 
  microM.ircommand=0;                          // prevents display repeating the same values
}




/*
=================================================== Available Global Variables =====================================================
 
 microM.irpin
 A byte value specifying the digital pin used by IR receiver (default value is 4).
 Only required if using an external IR receiver.
 
 */









