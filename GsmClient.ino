/*
 * ArduinoProject GPRS Shield control.
 *
 * Initializes the GPRS Shield when it's turned on
 * and handles received SMS messages.
 *
 * @date 02/2012
 * @author Markus Lindqvist
 */
 
#include <SoftwareSerial.h>

/** What we need is an Arduino Sketch running inside
  * the ATmega328P that would emulate a second serial
  * port (UART) using software on the digital pins D8 and D7
  * See: http://www.seeedstudio.com/wiki/GPRS_Shield#Step_1:_Creating_a_test_setup_for_the_GPRS_Shield */
SoftwareSerial gsmSerial(7, 8);

const int _gsmTimeout = 30;
const int _gsmMaxBuffer = 128;
char _gsmBuffer[_gsmMaxBuffer] = "";

const int _gprsShieldFactoryDefaultBps = 19200;

void initializeGsm()
{
  debug("initializeGsm");
  gsmSerial.begin(_gprsShieldFactoryDefaultBps);
}

void _writePinCode()
{
  gsmSerial.write("AT+CPIN=7201\r");
  debug2("PIN code written");
  _gsmReadBytesOrDisplayError(4); // AT\r\n
}

void _receiveTextMessage()
{
  debug("RecvSms: NOT IMPLEMENTED");
}

void _gsmSerialHandleLine()
{
  String s = _gsmBuffer;
  debug(s);

  if(s.indexOf("+CPIN: SIM PIN") != -1)
  {
    _writePinCode();
  }
  else if(s.indexOf("+CMTI:") != -1) // +CMTI: "SM",1
  {
    _receiveTextMessage();
  }
  else
  {
    // Currently unhandled!
  }
}
 
void gsmPollContent()
{
  if(gsmSerial.available())
  {
    byte charsRead = gsmSerial.readBytesUntil('\n', _gsmBuffer, _gsmMaxBuffer);
    if(charsRead)
    {
      _gsmBuffer[charsRead] = 0; // Terminate string
      _gsmSerialHandleLine();
    }
  }
}

boolean _gsmReadBytesOrDisplayError(char numberOfBytes)
{
  if(!_gsmWaitForBytes(numberOfBytes, _gsmTimeout))
  {
    debug("Error reading bytes from device!");
    return false;
  }
  
  while(gsmSerial.available())
  {
    char next = gsmSerial.read();
    Serial.print(next);
  }
  
  return true;
}

char _gsmWaitForBytes(char numberOfBytes, int timeout)
{
  while(gsmSerial.available() < numberOfBytes)
  {
    delay(200);
    timeout -= 1;
    if(timeout == 0)
    {
      return 0;
    }
  }
  return 1;
}

