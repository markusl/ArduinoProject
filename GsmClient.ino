/*
 * ArduinoProject GPRS Shield control.
 *
 * Initializes the GPRS Shield when it's turned on
 * and handles received SMS messages.
 *
 * @date 02/2012
 * @author Markus Lindqvist
 */
 
#include <SoftwareSerial.h> // Remember to patch!

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

void _deviceSettings()
{
  gsmSerial.write("AT+CNMI=3,1\r"); // Expect +CMTI when new message arrives
  _gsmReadBytesOrDisplayError(4); // OK\r\n
  gsmSerial.write("AT+CMGF=1\r"); // Text mode
  _gsmReadBytesOrDisplayError(4); // OK\r\n
}

void _writePinCode()
{
  gsmSerial.write("AT+CPIN=7201\r");
  debug2("PIN code written");
  delay(200);
  _gsmReadBytesOrDisplayError(4); // OK\r\n
  
  delay(200);
  _deviceSettings();
  
  delay(200);
  _requestSmsAtIndex(String("1"));
}

void _requestSmsAtIndex(const String &index)
{
  gsmSerial.print("AT+CMGR=" + index + "\r");
}

void _receiveTextMessage(const String &line)
{
  String index = line.substring(12);
  index.trim();
  debug("Got index " + index);
  gsmSerial.flush();
  _requestSmsAtIndex(index);
}

boolean _isNewSms(const String &line)
{
  // +CMTI: "SM",11
  return line.indexOf("+CMTI") != -1 &&
         line.length() > 12;
}

boolean _isSms(const String &line)
{
  return line.indexOf("+CMGR") != -1;
}

boolean _readAndPrintSms()
{
  // Line contains the +CMGR response, now we need to read the message
  byte charsRead = gsmSerial.readBytesUntil('\n', _gsmBuffer, _gsmMaxBuffer);
  if(charsRead > 1)
  {
    charsRead--; // Skip newline
    _gsmBuffer[charsRead] = 0; // Terminate string
    debug("Message content" + String(_gsmBuffer));
    _gsmReadBytesOrDisplayError(4); // OK\r\n
    lcdPrintString(String(_gsmBuffer));
  }
}

void _gsmSerialHandleLine(const String &s)
{
  debug(s);

  if(s.indexOf("+CPIN: SIM PIN") != -1)
  {
    _writePinCode();
  }
  else if(_isNewSms(s)) // +CMTI: "SM",1
  {
    _receiveTextMessage(s);
  }
  else if(_isSms(s)) // +CMGR: "REC READ","+358407075732","","12/02/22,17:58:00+08"
  {
    _readAndPrintSms();
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
      _gsmSerialHandleLine(String(_gsmBuffer));
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

