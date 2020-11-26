// This example shows how to control the Romi 32U4 with an
// infrared (IR) remote control and an IR receiver module.
//
// This code is designed to work with the Mini IR Remote Control
// and the Vishay TSOP38438 IR receiver module, both available
// from Pololu:
//
//   https://www.pololu.com/product/2777
//   https://www.pololu.com/product/2471
//
// You will need to make the following connections between the IR
// receiver and the Romi 32U4:
//
// - IR receiver OUT to Romi 32U4 pin 0
// - IR recevier GND to Romi 32U4 GND
// - IR receiver VS to Romi 32U4 5V
//
// The arrow buttons control the robot's movement, while the
// number buttons from 1 to 3 play different notes on the buzzer.
//
// To change what actions are performed when a button press is
// detected, you should change the processRemoteCommand and
// stopCurrentCommand functions.
//
// If you have a different remote that uses the NEC protocol with
// a 38 kHz, 940 nm infrared emitter, it should be possible to
// make it instead of the mini remote from Pololu.  You can use
// this code to decode the messages from your remote, and then
// you can edit the constants in RemoteConstants.h to match what
// was transmitted from your remote.

#include <Romi32U4.h>
#include "RemoteConstants.h"
#include "RemoteDecoder.h"

// This variable sets the amount of time (in milliseconds) that
// we wait before considering the current message from the remote
// to have expired.  This type of remote typically sends a repeat
// command every 109 ms, so a timeout value of 115 was chosen.
// You can increase this timeout to 230 if you want to be more
// tolerant of errors that occur while you are holding down the
// button, but it will make the robot slower to respond when you
// release the button.
const uint16_t messageTimeoutMs = 115;

// This variable is true if the last message received from the
// remote is still active, meaning that we are still performing
// the action specified by the message.  A message will be active
// if the remote button is being held down and the robot has been
// successfully receiving messages from the remote since the
// button was pressed.
bool messageActive = false;

// This is the time that the current message from the remote was
// last verified, in milliseconds.  It is used to implement the
// timeout defined above.
uint16_t lastMessageTimeMs = 0;

Romi32U4Motors motors;
Romi32U4ButtonA buttonA;

RemoteDecoder decoder;

// Initializes the IR sesnor by enabling a pull-up resistor for
// it.
void irSensorInit()
{
  FastGPIO::Pin<0>::setInputPulledUp();
}

// Reads from the IR sensor.  If the pin is low then the sensor
// is detecting something and this function returns true.
bool irSensorRead()
{
  return !FastGPIO::Pin<0>::isInputHigh();
}

void setup()
{
  irSensorInit();
}

void loop()
{
  decoder.service(irSensorRead());

  // Turn on the yellow LED if a message is active.
  ledYellow(messageActive);

  // Turn on the red LED if we are in the middle of receiving a
  // new message from the remote.  You should see the red LED
  // blinking about 9 times per second while you hold a remote
  // button down.
  ledRed(decoder.criticalTime());

  if (decoder.criticalTime())
  {
    // We are in the middle of receiving a message from the
    // remote, so we should avoid doing anything that might take
    // more than a few tens of microseconds, and call
    // decoder.service() as often as possible.
  }
  else
  {
    // We are not in a critical time, so we can do other things
    // as long as they do not take longer than about 7.3 ms.
    // Delays longer than that can cause some remote control
    // messages to be missed.

    processRemoteEvents();
  }

  // Check how long ago the current message was last verified.
  // If it is longer than the timeout time, then the message has
  // expired and we should stop executing it.
  if (messageActive && (uint16_t)(millis() - lastMessageTimeMs) > messageTimeoutMs)
  {
    messageActive = false;
    stopCurrentCommand();
  }
}

void processRemoteEvents()
{
  if (decoder.getAndResetMessageFlag())
  {
    // The remote decoder received a new message, so record what
    // time it was received and process it.
    lastMessageTimeMs = millis();
    messageActive = true;
    processRemoteMessage(decoder.getMessage());
  }

  if (decoder.getAndResetRepeatFlag())
  {
    // The remote decoder receiver a "repeat" command, which is
    // sent about every 109 ms while the button is being held
    // down.  It contains no data.  We record what time the
    // repeat command was received so we can know that the
    // current message is still active.
    lastMessageTimeMs = millis();
  }
}

void processRemoteMessage(const uint8_t * message)
{
  // Print the raw message on the first line of the SM, in hex.
  // The first two bytes are usually an address, and the third
  // byte is usually a command.  The last byte is supposed to be
  // the bitwise inverse of the third byte, and if that is the
  // case, then we don't print it.
  char buffer[9];
  if (message[2] + message[3] == 0xFF)
  {
    sprintf(buffer, "%02X%02X %02X ",
      message[0], message[1], message[2]);
  }
  else
  {
    sprintf(buffer, "%02X%02X%02X%02X",
      message[0], message[1], message[2], message[3]);
  }
  Serial.print(buffer);

  // Make sure the address matches what we expect.
  if (message[0] != remoteAddressByte0 ||
    message[1] != remoteAddressByte1)
  {
    Serial.print(F("bad addr"));
    return;
  }

  // Make sure that the last byte is the logical inverse of the
  // command byte.
  if (message[2] + message[3] != 0xFF)
  {
    Serial.print(F("bad cmd"));
    return;
  }

  stopCurrentCommand();
  processRemoteCommand(message[2]);
}

// Start running the new command.
void processRemoteCommand(uint8_t command)
{
  switch(command)
  {
  case remoteUp:
    Serial.print(F("up"));
    motors.setEfforts(300, 300);
    break;

  case remoteDown:
    Serial.print(F("down"));
    motors.setEfforts(-300, -300);
    break;

  case remoteLeft:
    Serial.print(F("left"));
    motors.setEfforts(-200, 200);
    break;

  case remoteRight:
    Serial.print(F("right"));
    motors.setEfforts(200, -200);
    break;

  case remoteStopMode:
    Serial.print(F("stop"));
    break;

  case remoteEnterSave:
    Serial.print(F("enter"));
    break;

  case remoteVolMinus:
    Serial.print(F("vol-"));
    break;

  case remoteVolPlus:
    Serial.print(F("vol+"));
    break;

  case remotePlayPause:
    Serial.print(F("play"));
    break;

  case remoteSetup:
    Serial.print(F("setup"));
    break;

  case remoteBack:
    Serial.print(F("back"));
    break;

  case remote0:
    Serial.print(F("0"));
    break;

  case remote1:
    Serial.print(F("1"));
    break;

  case remote2:
    Serial.print(F("2"));
    break;

  case remote3:
    Serial.print(F("3"));
    break;

  case remote4:
    Serial.print(F("4"));
    break;

  case remote5:
    Serial.print(F("5"));
    break;

  case remote6:
    Serial.print(F("6"));
    break;

  case remote7:
    Serial.print(F("7"));
    break;

  case remote8:
    Serial.print(F("8"));
    break;

  case remote9:
    Serial.print(F("9"));
    break;
  }
}

// Stops the current remote control command.  This is called when
// a new command is received or if the current command has
// expired.
void stopCurrentCommand()
{
  motors.setEfforts(0, 0);
}
