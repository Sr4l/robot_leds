#include "Adafruit_NeoPixel.h"
#include "LedStripe.h"


/* *************************************
 * dokumentation
 * ************************************ */
// version: 24-may-16
//
// changelog
// version 1: with LED library fastLED
// version 2: ported to Adafruit NeoPixel (because it is much smaller and faster)
// version 3: support multiple different LED stripes and animations

/* *************************************
 * some defines
 * ************************************ */
#define resetCommandBuffer() incoming_command = ""

/* *************************************
 * configuration
 * ************************************ */
#define SERIAL_SPEED 9600
#define NUM_LEDS_BODEN 18
#define NUM_LEDS_BECHER 16
#define DIGITAL_PIN_BODEN 6
#define DIGITAL_PIN_BECHER 7
#define COMMAND_BUFFER_LENGTH 254
#define COMMAND_SEPERATOR 59  // ASCII tabele: 59 = ';'
#define ARGUMENT_SEPERATOR 44 // ASCII table: 44 = ','
#define ARGUMENT_LENGTH 6

/* *************************************
 * global variables
 * ************************************ */

String incoming_command = "";
LedStripe leds_boden(NUM_LEDS_BODEN, DIGITAL_PIN_BODEN);
LedStripe leds_becher(NUM_LEDS_BECHER, DIGITAL_PIN_BECHER);

/* *************************************
 * parse incomming command
 * ************************************ */
void parseCommand(uint8_t *args)
{  
  Serial.println("Raw: " + incoming_command);
  
  uint8_t i = 0;  
  for (uint8_t j = 0; j < ARGUMENT_LENGTH; j++)
  {
    uint8_t sep = incoming_command.indexOf(ARGUMENT_SEPERATOR, i);
    String tmp = incoming_command.substring(i, sep);
    i = sep + 1;
    args[j] = tmp.toInt();
    
    //Serial.println("Argument: " + tmp);
    if (sep == 255) break;
  }
}

/* *************************************
 * start serial connection and wait for
 * incomming uint8_t
 * ************************************ */
void startAndWaitForSerialConnection()
{
  // start serial port at SERIAL_SPEED bps:
  Serial.begin(SERIAL_SPEED);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  while (Serial.available() <= 0) {
    Serial.println("Wait for initial command (any uint8_t)!");
    delay(500);
  }
  
  Serial.read();
  Serial.println("Ready for use! Send: LED selector (0 - led stipe a,  1- led stripe b), "
       "animation mode, animation parameter, H, S, V; (i.e 0,0,10,64,255,255;)");
}

/* *************************************
 * setup (init) function
 * ************************************ */
void setup()
{
  // wait for Roboter to connect, this keeps LEDs off)
  startAndWaitForSerialConnection();
}

/* *************************************
 * main loop function
 * ************************************ */
void loop()
{
  int inbyte = Serial.read();
  uint8_t args[ARGUMENT_LENGTH];
  
  
  if (inbyte != -1)
  {
    if (inbyte == COMMAND_SEPERATOR)
    {
      parseCommand(args);
      resetCommandBuffer();
      if (args[0] == 0)
      {
        leds_boden.update(args[1], args[2], args[3], args[4], args[5]);
      }
      else if (args[0] == 1)
        leds_becher.update(args[1], args[2], args[3], args[4], args[5]);
    }
    else if (isDigit(inbyte) || inbyte == ARGUMENT_SEPERATOR)
    {
      incoming_command += (char)inbyte;
    } else {
      Serial.println("Invaild Character! Reset command buffer.  Digits," + 
            String(ARGUMENT_SEPERATOR) + " and " + String(COMMAND_SEPERATOR) + "allowed.");
      resetCommandBuffer();
    }
  }
  
  leds_boden.cycle();
  leds_becher.cycle();
}
